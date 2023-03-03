/******************************************************************************
* File Name: main.c
*
* Description: This is the source code for PMG1 On-chip temp sensor 12-bit SAR ADC Example
*              for ModusToolbox.
*
* Related Document: See README.md
*
*******************************************************************************
* Copyright 2022-2023, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/


/*******************************************************************************
 * Include header files
 ******************************************************************************/
#include "cybsp.h"
#include "cy_pdl.h"
#include "stdio.h"
#include <inttypes.h>

/*******************************************************************************
 * Macros
 ******************************************************************************/
#define CY_ASSERT_FAILED            (0u)
#define SW_DEBOUNCE_DELAY           (25u)    /* 25 milliseconds */
#define LED_FLASH_DELAY             (50u)    /* 50 milliseconds */

/* Debug print macro to enable the UART print */
#define DEBUG_PRINT                 (0u)

/*******************************************************************************
* Global Variable
*******************************************************************************/
cy_stc_scb_uart_context_t UART_context;

/* User Switch Interrupt Configuration */
const cy_stc_sysint_t User_Switch_intr_config =
{
    .intrSrc = CYBSP_USER_SW_IRQ,       /* Source of interrupt signal */
    .intrPriority = 3u,                 /* Interrupt priority */
};

/* Flag to detect switch press event */
volatile uint8_t SwitchPressFlag = 0;

void User_Switch_Interrupt_Handler(void);

#if DEBUG_PRINT
/* Variable used for tracking the print status */
volatile bool ENTER_LOOP = true;

/*******************************************************************************
* Function Name: check_status
********************************************************************************
* Summary:
*  Prints the error message.
*
* Parameters:
*  error_msg - message to print if any error encountered.
*  status - status obtained after evaluation.
*
* Return:
*  void
*
*******************************************************************************/
void check_status(char *message, cy_rslt_t status)
{
    char error_msg[50];

    sprintf(error_msg, "Error Code: 0x%08" PRIX32 "\n", status);

    Cy_SCB_UART_PutString(CYBSP_UART_HW, "\r\n=====================================================\r\n");
    Cy_SCB_UART_PutString(CYBSP_UART_HW, "\nFAIL: ");
    Cy_SCB_UART_PutString(CYBSP_UART_HW, message);
    Cy_SCB_UART_PutString(CYBSP_UART_HW, "\r\n");
    Cy_SCB_UART_PutString(CYBSP_UART_HW, error_msg);
    Cy_SCB_UART_PutString(CYBSP_UART_HW, "\r\n=====================================================\r\n");
}
#endif

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*  System entrance point. This function performs
*  - initial setup of device
*  - initialize UART_SCB, 12-bit SAR ADC and external interrupt for Switch
*  - check for user switch press, user flash LED, read ADC value and calculate the die-temperature
*  - send the temperature value through UART_SCB
*
* Parameters:
*  none
*
* Return:
*  int
*
*******************************************************************************/

int main(void)
{
    cy_rslt_t result;
    cy_en_sysint_status_t intr_result;
    SFLASH_Type *sflash = SFLASH;

    int16_t ADC_injResult;    /* Variable storing ADC injection channel value */
    char_t temp_string[50];    /* Variable storing temperature data as a string for UART transmission */
    int temp_initial_1;    /* 'int' occupies 32-bits here*/
    int temp_initial_2;
    int temp_adjust;    /* Temperature error compensation value */
    int temperature;
    int temp_integer;    /* Integer part of temperature value */
    int temp_decimal;    /* Decimal part of temperature value */

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(CY_ASSERT_FAILED);
    }

    /* Configure and enable the UART peripheral */
    Cy_SCB_UART_Init(CYBSP_UART_HW, &CYBSP_UART_config, &UART_context);
    Cy_SCB_UART_Enable(CYBSP_UART_HW);

#if DEBUG_PRINT
    /* Sequence to clear screen */
    Cy_SCB_UART_PutString(CYBSP_UART_HW, "\x1b[2J\x1b[;H");

    /* Print "On-chip temp sensor 12-bit SAR ADC" */
    Cy_SCB_UART_PutString(CYBSP_UART_HW, "****************** ");
    Cy_SCB_UART_PutString(CYBSP_UART_HW, "PMG1 MCU: On-chip temp sensor 12-bit SAR ADC");
    Cy_SCB_UART_PutString(CYBSP_UART_HW, "***************** \r\n\n");
#endif

    /* SAR ADC initialization */
    cy_en_sar_status_t sarStatus = Cy_SAR_Init(SAR0, &CYBSP_ADC0_config);
    if (sarStatus != CY_SAR_SUCCESS  )
    {
#if DEBUG_PRINT
        check_status("API Cy_SAR_Init failed with error code", sarStatus);
#endif
        /* Insert error handling here */
        CY_ASSERT(CY_ASSERT_FAILED);
    }

    /* SAR ADC enabling */
    Cy_SAR_Enable(SAR0);

    /* Initialize Switch GPIO interrupt */
    intr_result = Cy_SysInt_Init(&User_Switch_intr_config, &User_Switch_Interrupt_Handler);
    if (intr_result != CY_SYSINT_SUCCESS)
    {
#if DEBUG_PRINT
        check_status("API Cy_SysInt_Init failed with error code", intr_result);
#endif
        /* Insert error handling here */
        CY_ASSERT(CY_ASSERT_FAILED);
    }

    /* Clear any pending interrupt and enable the User Switch Interrupt */
    NVIC_ClearPendingIRQ(User_Switch_intr_config.intrSrc);
    NVIC_EnableIRQ(User_Switch_intr_config.intrSrc);

    /* Enable global interrupts */
    __enable_irq();

    /* Send a string over serial terminal */
    Cy_SCB_UART_PutString(CYBSP_UART_HW,"\n Press user switch (SW2) to display the Die-Temperature: \r\n\n");

    for(;;)
    {
        if(SwitchPressFlag)
        {
            /* Wait for 25 milliseconds for switch de-bounce*/
            Cy_SysLib_Delay(SW_DEBOUNCE_DELAY);

            if(!Cy_GPIO_Read(CYBSP_USER_SW_PORT, CYBSP_USER_SW_PIN))
            {
                Cy_SAR_EnableInjection(SAR0, true);
                /* Scenario: execute the injection channel at the tail of the nearest ADC scanning */

                /* Trigger SAR conversion */
                Cy_SAR_StartConvert(SAR0, CY_SAR_START_CONVERT_SINGLE_SHOT);

                /* 2.304 milliseconds delay is required to finish an ADC sample conversion
                 * This delay interval may be utilized effectively by inserting user-defined code as required by the application */
                /* 'CY_SAR_RETURN_STATUS_INJ' immediately returns the conversion status of the injection channel.
                 * This mode is non-blocking
                 * Hence, here a while-loop is used to check the conversion status before reading the ADC code */
                while (CY_SAR_SUCCESS != Cy_SAR_IsEndConversion(SAR0, CY_SAR_RETURN_STATUS_INJ))
                {

                }    
                ADC_injResult = Cy_SAR_GetResult16(SAR0, CY_SAR_INJ_CHANNEL);
                /* now ADC_injResult contains the injection channel result */

                /* Temperature conversion algorithm*/
                temp_initial_1 = ((int16_t)sflash->SAR_TEMP_MULTIPLIER) * ADC_injResult + 1024 * ((int16_t)sflash->SAR_TEMP_OFFSET);    /* Temperature value with slope error */
                temp_initial_2 = (temp_initial_1 >> 16) * 65536;    /* Upper 16 bits x 2^16 */
                                                                    /* 2^16 = 65536 */
                if ((temp_initial_1 >> 16) > 15)                    /* If temperature > 15C */
                {
                    temp_adjust = (int)((0.5/85)*(100*(65536) - temp_initial_2));    /* Temperature error compensation value */
                }
                else                                                /* If temperature <= 15C */
                {
                    temp_adjust = (int)((0.5/55)*(40*(65536) - temp_initial_2));    /* Temperature error compensation value */
                }

                temperature = temp_initial_1 + temp_adjust;    /* Compensated value of temperature */
                temp_integer = temperature >> 16;    /* Integer part of the temperature */
                temp_decimal = temperature & 0x0000FFFF;    /* Decimal part of the temperature; Mask: 0x0000FFFF = 65535 */

                /* Conversion from int to char_t */
                sprintf(temp_string,"Temperature = %d.%d C \r\n\n", temp_integer, temp_decimal);

                /* Turn ON the user LED */
                Cy_GPIO_Clr(CYBSP_USER_LED_PORT,CYBSP_USER_LED_PIN);
                /* 50 milliseconds delay is used here to allow a flashing action on the user LED */
                Cy_SysLib_Delay(LED_FLASH_DELAY);
                /* Turn OFF the user LED */
                Cy_GPIO_Set(CYBSP_USER_LED_PORT,CYBSP_USER_LED_PIN);

                /* Send a string over serial terminal */
                Cy_SCB_UART_PutString(CYBSP_UART_HW, temp_string);
            }
            /* Clear the Switch Press Event */
            SwitchPressFlag = 0;
        }
#if DEBUG_PRINT
        if (ENTER_LOOP)
        {
            Cy_SCB_UART_PutString(CYBSP_UART_HW, "Entered for loop \r\n");
            ENTER_LOOP = false;
        }
#endif
    }
}

void User_Switch_Interrupt_Handler(void)
{
    /* Set Switch press flag to 1 */
    SwitchPressFlag = 1;

    /* Clear the Interrupt */
    Cy_GPIO_ClearInterrupt(CYBSP_USER_SW_PORT, CYBSP_USER_SW_NUM);
}

/* [] END OF FILE */

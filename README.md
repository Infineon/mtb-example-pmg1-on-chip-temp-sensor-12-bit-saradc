# EZ-PD&trade; PMG1 MCU: On-chip temp sensor 12-bit SAR ADC

This code example demonstrates the method of reading internal temperature of the EZ-PD&trade; PMG1-S3 MCU device through the BJT-based on-chip temperature sensor and the 12-bit SAR ADC. The temperature value is displayed on a UART terminal when required, using the user switch. Additionally, it also explains the configuration and analog routing of PASS 0 Temperature Sensor 0 using Device Configurator in ModusToolbox&trade; software.

[View this README on GitHub.](https://github.com/Infineon/mtb-example-pmg1-on-chip-temp-sensor-12-bit-saradc)

[Provide feedback on this code example.](https://cypress.co1.qualtrics.com/jfe/form/SV_1NTns53sK2yiljn?Q_EED=eyJVbmlxdWUgRG9jIElkIjoiQ0UyMzUwMzkiLCJTcGVjIE51bWJlciI6IjAwMi0zNTAzOSIsIkRvYyBUaXRsZSI6IkVaLVBEJnRyYWRlOyBQTUcxIE1DVTogT24tY2hpcCB0ZW1wIHNlbnNvciAxMi1iaXQgU0FSIEFEQyIsInJpZCI6ImVhc292YXJnaGVzZSIsIkRvYyB2ZXJzaW9uIjoiMS4wLjAiLCJEb2MgTGFuZ3VhZ2UiOiJFbmdsaXNoIiwiRG9jIERpdmlzaW9uIjoiTUNEIiwiRG9jIEJVIjoiV0lSRUQiLCJEb2MgRmFtaWx5IjoiVFlQRS1DIn0=)


## Requirements

- [ModusToolbox&trade; software](https://www.cypress.com/products/modustoolbox-software-environment) v2.4 or later
- Set the `CY_SUPPORTED_KITS` environment variable with the value of "PMG1" to enable support for the PMG1 parts and BSPs in ModusToolbox&trade; software
- Board support package (BSP) minimum required version: 1.2.0
- Programming language: C
- Associated parts: [EZ-PD&trade; PMG1-S3 MCU](http://www.cypress.com/PMG1)


## Supported toolchains (make variable 'TOOLCHAIN')

- GNU Arm® embedded compiler v9.3.1 (`GCC_ARM`) - Default value of `TOOLCHAIN`
- Arm&reg; compiler v6.13 (`ARM`)
- IAR C/C++ compiler v8.42.2 (`IAR`)

## Supported kits (make variable 'TARGET')

- [EZ-PD&trade; PMG1-S3 prototyping kit](http://www.cypress.com/CY7113) (`PMG1-CY7113`) – Default value of `TARGET`



## Hardware setup

1. Connect the board to your PC using a USB cable through the KitProg3 USB connector (J1). This cable is used for programming the PMG1 device and can be used during debugging. During operation, it transfers the UART data from the serial port to the PC, to display it on a serial monitor.

2. Connect the USB PD port (J10) to a USB-C power adapter/USB port on PC using a Type-C/Type-A to Type-C cable to power the PMG1 device for normal operation.

3. For PMG1-S3 kits of revision 3 or lower, connect the UART Tx (J6.10) and UART Rx (J6.9) lines from the PMG1 kit to J3.8 and J3.10 on KitProg3 respectively, to establish a UART connection between KitProg3 and the PMG1 device. Kits with a higher revision have UART lines internally connected and hence external wiring is not required. Note that, in this application, UART Tx line alone will be used to transmit the temperature data onto serial monitor.



See the kit user guide for details on configuring the board.


## Software setup

Install a terminal emulator to display the serial data. Instructions in this document use [Tera Term](https://ttssh2.osdn.jp/index.html.en).

This example requires no additional software or tools.


## Using the code example

Create the project and open it using one of the following:

<details><summary><b>In Eclipse IDE for ModusToolbox&trade; software</b></summary>

1. Click the **New Application** link in the **Quick Panel** (or, use **File** > **New** > **ModusToolbox Application**). This launches the [Project Creator](https://www.cypress.com/ModusToolboxProjectCreator) tool.

2. Pick a kit supported by the code example from the list shown in the **Project Creator - Choose Board Support Package (BSP)** dialog.

   When you select a supported kit, the example is reconfigured automatically to work with the kit. To work with a different supported kit later, use the [Library Manager](https://www.cypress.com/ModusToolboxLibraryManager) to choose the BSP for the supported kit. You can use the Library Manager to select or update the BSP and firmware libraries used in this application. To access the Library Manager, click the link from the **Quick Panel**.

   You can also just start the application creation process again and select a different kit.

   If you want to use the application for a kit not listed here, you may need to update the source files. If the kit does not have the required resources, the application may not work.

3. In the **Project Creator - Select Application** dialog, choose the example by enabling the checkbox.

4. (Optional) Change the suggested **New Application Name**.

5. The **Application(s) Root Path** defaults to the Eclipse workspace which is usually the desired location for the application. If you want to store the application in a different location, you can change the *Application(s) Root Path* value. Applications that share libraries should be in the same root path.

6. Click **Create** to complete the application creation process.

For more details, see the [Eclipse IDE for ModusToolbox&trade; software user guide](https://www.cypress.com/MTBEclipseIDEUserGuide) (locally available at *{ModusToolbox&trade; software install directory}/ide_{version}/docs/mt_ide_user_guide.pdf*).

</details>

<details><summary><b>In command-line interface (CLI)</b></summary>

ModusToolbox&trade; software provides the Project Creator as both a GUI tool and the command line tool, "project-creator-cli". The CLI tool can be used to create applications from a CLI terminal or from within batch files or shell scripts. This tool is available in the *{ModusToolbox&trade; software install directory}/tools_{version}/project-creator/* directory.

Use a CLI terminal to invoke the "project-creator-cli" tool. On Windows, use the command line "modus-shell" program provided in the ModusToolbox&trade; software installation instead of a standard Windows command-line application. This shell provides access to all ModusToolbox&trade; software tools. You can access it by typing `modus-shell` in the search box in the Windows menu. In Linux and macOS, you can use any terminal application.

This tool has the following arguments:

Argument | Description | Required/optional
---------|-------------|-----------
`--board-id` | Defined in the `<id>` field of the [BSP](https://github.com/Infineon?q=bsp-manifest&type=&language=&sort=) manifest | Required
`--app-id`   | Defined in the `<id>` field of the [CE](https://github.com/Infineon?q=ce-manifest&type=&language=&sort=) manifest | Required
`--target-dir`| Specify the directory in which the application is to be created if you prefer not to use the default current working directory | Optional
`--user-app-name`| Specify the name of the application if you prefer to have a name other than the example's default name | Optional

<br>

The following example will clone the "[Hello World](https://github.com/Infineon/mtb-example-pmg1-hello-world)" application with the desired name "MyHelloWorld" configured for the *PMG1-CY7110* BSP into the specified working directory, *C:/mtb_projects*:

   ```
   project-creator-cli --board-id PMG1-CY7110 --app-id mtb-example-pmg1-hello-world --user-app-name MyHelloWorld --target-dir "C:/mtb_projects"
   ```

**Note:** The project-creator-cli tool uses the `git clone` and `make getlibs` commands to fetch the repository and import the required libraries. For details, see the "Project Creator tools" section of the [ModusToolbox&trade; software user guide](https://www.cypress.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox&trade; software install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>

<details><summary><b>In third-party IDEs</b></summary>

Use one of the following options:

- **Use the standalone [Project Creator](https://www.cypress.com/ModusToolboxProjectCreator) tool:**

   1. Launch Project Creator from the Windows Start menu or from *{ModusToolbox&trade; software install directory}/tools_{version}/project-creator/project-creator.exe*.

   2. In the initial **Choose Board Support Package** screen, select the BSP, and click **Next**.

   3. In the **Select Application** screen, select the appropriate IDE from the **Target IDE** drop-down menu.

   4. Click **Create** and follow the instructions printed in the bottom pane to import or open the exported project in the respective IDE.

<br>

- **Use command-line interface (CLI):**

   1. Follow the instructions from the **In command-line interface (CLI)** section to create the application, and then import the libraries using the `make getlibs` command.

   2. Export the application to a supported IDE using the `make <ide>` command.

   3. Follow the instructions displayed in the terminal to create or import the application as an IDE project.

For a list of supported IDEs and more details, see the "Exporting to IDEs" section of the [ModusToolbox&trade; software user guide](https://www.cypress.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox&trade; software install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>


## Operation

1. Ensure that the steps listed in the [Hardware setup](#hardware-setup) section are completed.

2. Ensure that the jumper shunt on power selection jumper (J5) is placed at position 2-3 to enable programming mode.

3. Connect the board to your PC using the USB cable through the KitProg3 USB connector (J1). This cable is used for programming the PMG1 device.

4. Program the board using one of the following:

   <details><summary><b>Using Eclipse IDE for ModusToolbox&trade; software</b></summary>

      1. Select the application project in the Project Explorer.

      2. In the **Quick Panel**, scroll down, and click **\<Application Name> Program (KitProg3_MiniProg4)**.
   </details>

   	<details><summary><b>Using CLI</b></summary>

     From the terminal, execute the `make program` command to build and program the application using the default toolchain to the default target. The default toolchain and target are specified in the application's Makefile but you can override those values manually:
      ```
      make program TARGET=<BSP> TOOLCHAIN=<toolchain>
      ```

      Example:
      ```
      make program TARGET=PMG1-CY7113 TOOLCHAIN=GCC_ARM
      ```
     **Note:** This application supports only the PMG1-CY7113 BSP.
   </details>

5. After programming the kit, disconnect the USB cable and change the position on the power selection jumper (J5) to 1-2 to power the kit through the USB PD port in operational mode.

6. Now, reconnect the USB cable to KitProg3 Type-C port (J1). Open a terminal program and select the KitProg3 COM port. Set the serial port parameters to 8N1 and 115200 baud.

7. Power the kit through the USB PD port (J10), using the second USB cable.

8. As soon as the kit is powered through the USB PD port, the application prints "Press user switch (SW2) to display the Die-Temperature:". Press the user switch (SW2) to display the value of die-temperature. Note that the user LED (LED3) flashes each time, indicating a new output display on the UART terminal.


**Figure 1. UART terminal display of temperature data**

<img src = "images/terminal-display.png" width = "500"/>


## Debugging

You can debug the example to step through the code. In the IDE, use the **\<Application name> Debug (KitProg3_MiniProg4)** configuration in the **Quick Panel**.

Ensure that the board is connected to your PC using the USB cables through both the KitProg3 USB connector as well as the USB PD port, with the jumper shunt on power selection jumper (J5) placed at position 1-2.

See the "Debug mode" section in the kit user guide for debugging the application on the CY7113 prototyping kit. For more details, see the "Program and debug" section in the [Eclipse IDE for ModusToolbox&trade; software user guide](https://www.cypress.com/MTBEclipseIDEUserGuide).


## Design and implementation

This code example uses PASS 0 Temperature Sensor 0, a BJT-based internal temperature sensor located along with SARMUX in the programmable analog block (only in PMG1-S3 devices).

Here, the BJT device is connected in a diode configuration, whose base-to-emitter voltage (VBE) has a strong dependence on the junction temperature. The voltage VBE is then applied to the Vplus pin of the PASS 0 12-bit SAR ADC 0 as input in single-ended mode. The output codes of the ADC are then mapped to corresponding temperature values, using an appropriate temperature conversion algorithm. The 12-bit conversion provides a good average temperature resolution of around 0.5 celsius/LSB. The internal temperature sensor provides an accuracy of +/-5 celsius over the temperature range of -40C to +85C.

The temperature conversion algorithm used in this application has been detailed under the chapter 'Temperature sensor' in EZ-PD&trade; PMG1-S3 MCU Architecture Technical Reference Manual (TRM). The values of slope and offset used in the algorithm are determined using the characterization data of PMG1-S3 devices. The values of these constants are stored in the registers **SFLASH_SAR_TEMP_MULTIPLIER** and **SFLASH_SAR_TEMP_OFFSET** respectively. The addresses of these registers can be obtained from EZ-PD&trade; PMG1-S3 MCU Registers Technical Reference Manual (TRM).


### PASS 0 Temperature Sensor 0 configuration

Temperature sensing is done using the configurable PASS 0 12-bit SAR ADC 0 that can be configured using Device Configurator in ModusToolbox™ software as follows:

1. Select the application project in the Project Explorer.

2. In the **Quick Panel**, scroll down to **Tools** section, and click **Device Configurator 3.10**. This opens the Device Configurator window.

   **Figure 2. PASS 0 Temperature Sensor 0 configuration using Device Configurator 3.10**

   <img src = "images/temp-sensor-configuration-1.png" width = "1500"/>

3. Select **PASS 0 12-bit SAR ADC 0** under **Programmable Analog (PASS) 0** in the **Peripherals** tab. This opens the ADC configuration fields. Similarly, select **PASS 0 Temperature Sensor 0** to view the temperature sensor terminal connection.

4. The following are some of the ADC configuration parameters specific to the internal temperature sensor:

   - The temperature sensor is internally routed to the Vplus pin of the ADC by configuring the ADC in **Single-ended signed mode**. The Vneg pin of the ADC is connected to **VSSA**.

   - Reference voltage (Vref) used for this application is **Internal BandGap Reference** which is equal to 1.2V.

   - Vref **bypass capacitor** is not used for low-frequency operation of the ADC (between 1.7 MHz and 1.8 MHz).

   - **Injection channel** is enabled to achieve rare conversions, only when triggered. Note that at least one regular channel (0-15) must be enabled to use the injection channel. Also note that the injection channel, when triggered, will be scanned at the end of the next regular channel scan (tailgating).

   - ADC **clock frequency** is set to 1.778 MHz, using a suitable divider value, to enable operation without the Vref bypass capacitor. Note that any clock frequency greater than 1.8 MHz requires an external Vref bypass capacitor to be connected to the ext_vref pin for faithful code conversion.

   - Averaging is enabled for both Channel 0 and Injection Channel. **Samples Averaged** is set to 256 under 'Sampling' to minimize the effect of noise and other inherent errors.

   - **Full Resolution 12-bit** is chosen to ensure maximum performance of the ADC.

   - **Ch0 Vplus** and **Injection Vplus** are connected to **PASS 0 Temperature Sensor 0 temp**.


   **Figure 3. PASS 0 Temperature Sensor 0 configuration parameters**

   <img src = "images/temp-sensor-configuration-2.png" width = "1500"/>


   **Figure 4. Analog routing of temperature sensor in Device Configurator 3.10**

   <img src = "images/analog-routing.png" width = "1000"/>


   **Figure 5. Firmware flowchart**

   <img src = "images/firmware-flowchart.png" width = "600"/>


### Resources and settings

**Table 1. Application resources**

| Resource  |  Alias/object     |    Purpose     |
| :------- | :------------    | :------------ |
| Programmable Analog (PASS) 0    	| PASS 0 Temperature Sensor 0       	| BJT temperature sensor used to produce a temperature-dependent input voltage to the ADC|
| Programmable Analog (PASS) 0    	| PASS 0 12-bit SAR ADC 0       	| 12-bit SAR ADC used to convert the analog input voltage from the temperature sensor to digital codes|
| SCB 4			| CYBSP_UART			 	| UART SCB 4 block used for serial communication to send the temperature data through a serial port|
| Switch (BSP)	| CYBSP_USER_SW 	| User switch used as a trigger to display the temperature value 	|
| LED (BSP) 	| CYBSP_USER_LED 	| User LED to indicate the output 	|


## Related resources

Resources | Links
-----------|------------------
Application notes |[AN232553](https://www.cypress.com/an232553) – Getting started with EZ-PD&trade; PMG1 MCU on ModusToolbox&trade; software <br> [AN232565](https://www.cypress.com/an232565) – EZ-PD&trade; PMG1 MCU hardware design guidelines and checklist
Code examples | [Using ModusToolbox&trade; software](https://github.com/Infineon/Code-Examples-for-ModusToolbox-Software) on GitHub
Device documentation | [EZ-PD&trade; PMG1 MCU datasheets](https://www.cypress.com/PMG1DS)
Development kits | Select your kits from the [Evaluation Board Finder](https://www.infineon.com/cms/en/design-support/finder-selection-tools/product-finder/evaluation-board) page.
Libraries on GitHub | [mtb-pdl-cat2](https://github.com/infineon/mtb-pdl-cat2) – Peripheral driver library (PDL) and docs <br> [mtb-hal-cat2](https://github.com/infineon/mtb-hal-cat2) – Hardware abstraction layer (HAL) library and docs
Tools | [Eclipse IDE for ModusToolbox&trade; software](https://www.infineon.com/modustoolbox) <br> ModusToolbox&trade; software is a collection of easy-to-use software and tools enabling rapid development with Infineon&reg; MCUs, covering applications from embedded sense and control to wireless and cloud-connected systems using AIROC™ Wi-Fi & Bluetooth® combo devices.


## Other resources

Infineon provides a wealth of data at www.infineon.com to help you select the right device, and quickly and effectively integrate it into your design.

## Document history

Document title: *CE235039* – *EZ-PD&trade; PMG1 MCU: On-chip temp sensor 12-bit SAR ADC*

| Version | Description of change |
| ------- | --------------------- |
| 1.0.0   | New code example      |
------

All other trademarks or registered trademarks referenced herein are the property of their respective owners.

-------------------------------------------------------------------------------

© Cypress Semiconductor Corporation, 2022. This document is the property of Cypress Semiconductor Corporation, an Infineon Technologies company, and its affiliates (“Cypress”).  This document, including any software or firmware included or referenced in this document (“Software”), is owned by Cypress under the intellectual property laws and treaties of the United States and other countries worldwide.  Cypress reserves all rights under such laws and treaties and does not, except as specifically stated in this paragraph, grant any license under its patents, copyrights, trademarks, or other intellectual property rights.  If the Software is not accompanied by a license agreement and you do not otherwise have a written agreement with Cypress governing the use of the Software, then Cypress hereby grants you a personal, non-exclusive, nontransferable license (without the right to sublicense) (1) under its copyright rights in the Software (a) for Software provided in source code form, to modify and reproduce the Software solely for use with Cypress hardware products, only internally within your organization, and (b) to distribute the Software in binary code form externally to end users (either directly or indirectly through resellers and distributors), solely for use on Cypress hardware product units, and (2) under those claims of Cypress’s patents that are infringed by the Software (as provided by Cypress, unmodified) to make, use, distribute, and import the Software solely for use with Cypress hardware products.  Any other use, reproduction, modification, translation, or compilation of the Software is prohibited.
<br>
TO THE EXTENT PERMITTED BY APPLICABLE LAW, CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS DOCUMENT OR ANY SOFTWARE OR ACCOMPANYING HARDWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. No computing device can be absolutely secure. Therefore, despite security measures implemented in Cypress hardware or software products, Cypress shall have no liability arising out of any security breach, such as unauthorized access to or use of a Cypress product. CYPRESS DOES NOT REPRESENT, WARRANT, OR GUARANTEE THAT CYPRESS PRODUCTS, OR SYSTEMS CREATED USING CYPRESS PRODUCTS, WILL BE FREE FROM CORRUPTION, ATTACK, VIRUSES, INTERFERENCE, HACKING, DATA LOSS OR THEFT, OR OTHER SECURITY INTRUSION (collectively, “Security Breach”). Cypress disclaims any liability relating to any Security Breach, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any Security Breach. In addition, the products described in these materials may contain design defects or errors known as errata which may cause the product to deviate from published specifications. To the extent permitted by applicable law, Cypress reserves the right to make changes to this document without further notice. Cypress does not assume any liability arising out of the application or use of any product or circuit described in this document. Any information provided in this document, including any sample design information or programming code, is provided only for reference purposes. It is the responsibility of the user of this document to properly design, program, and test the functionality and safety of any application made of this information and any resulting product. “High-Risk Device” means any device or system whose failure could cause personal injury, death, or property damage. Examples of High-Risk Devices are weapons, nuclear installations, surgical implants, and other medical devices. “Critical Component” means any component of a High-Risk Device whose failure to perform can be reasonably expected to cause, directly or indirectly, the failure of the High-Risk Device, or to affect its safety or effectiveness. Cypress is not liable, in whole or in part, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any use of a Cypress product as a Critical Component in a High-Risk Device. You shall indemnify and hold Cypress, including its affiliates, and its directors, officers, employees, agents, distributors, and assigns harmless from and against all claims, costs, damages, and expenses, arising out of any claim, including claims for product liability, personal injury or death, or property damage arising from any use of a Cypress product as a Critical Component in a High-Risk Device. Cypress products are not intended or authorized for use as a Critical Component in any High-Risk Device except to the limited extent that (i) Cypress’s published data sheet for the product explicitly states Cypress has qualified the product for use in a specific High-Risk Device, or (ii) Cypress has given you advance written authorization to use the product as a Critical Component in the specific High-Risk Device and you have signed a separate indemnification agreement.
<br>
Cypress, the Cypress logo, and combinations thereof, WICED, ModusToolbox, PSoC, CapSense, EZ-USB, F-RAM, and Traveo are trademarks or registered trademarks of Cypress or a subsidiary of Cypress in the United States or in other countries. For a more complete list of Cypress trademarks, visit cypress.com. Other names and brands may be claimed as property of their respective owners.

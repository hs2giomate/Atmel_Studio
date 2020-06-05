===============
Low Power Modes
===============

This example demonstrates the different low power modes of the SAM
which include all or some of modes in IDLE, STANDBY, HIBERNATE, BACKUP and OFF.
An RTC is used to enter/exit the different modes every 15 seconds.
ACTIVE mode can also be measured.

Two ways to measure current consumptions:

1. Use Data Visualizer in Atmel Studio where power analysis window displays
   current power consumption, and have both CURRENT MEASUREMENT jumpers in
   MEASURE position.

2. Or connect an ammeter on CURRENT MEASUREMENT HEADER,
   and have both CURRENT MEASUREMENT jumpers in BYPASS position.

.. note::
   When IO jumper is MEASUREMENT the current consumption includes IO
   consumption. This makes measurement result huge. Setting IO jumper to BYPASS
   makes the measurement results closer to datasheet numbers.

.. note::
   On supported evaluation kits, there are different on-board components. They
   may take additional current. So test results varies on different boards and
   power modes, and can be larger than datasheet numbers. E.g., in OFF mode,
   on-board component is not reseted via IO pin and consumes a lot.

Drivers
-------
* Timer (RTC Lite)

Supported Evaluation Kit
------------------------
* SAM E54 Xplained Pro

Running the Demo
----------------
1. Download the selected example, or export the example to save the .atzip file.
2. Import .atzip file into Atmel Studio 7, File->Import->Atmel Start Project.
3. Build and flash into supported evaluation board.
4. Open Power debugger, Tools->Data Visualizer->Connect->Check Power->Start.
5. Press the RESET button.
6. The power analysis then displays.

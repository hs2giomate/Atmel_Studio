==========
DMA driver
==========

The four-channel direct memory access (DMA) controller can transfer data between memories and peripherals, and thus
off load these tasks from the CPU. It enables high data transfer rates with minimum CPU intervention, and frees up CPU
time. The four DMA channels enable up to four independent and parallel transfers.

The DMA controller can move data between SRAM and peripherals, between SRAM locations and directly between
peripheral registers. With access to all peripherals, the DMA controller can handle automatic transfer of data to/from
communication modules. The DMA controller can also read from memory mapped EEPROM.

Data transfers are done in continuous bursts of 1, 2, 4, or 8 bytes. They build block transfers of configurable size from 1
byte to 64KB. A repeat counter can be used to repeat each block transfer for single transactions up to 16MB. Source and
destination addressing can be static, incremental or decremental. Automatic reload of source and/or destination
addresses can be done after each burst or block transfer, or when a transaction is complete. Application software,
peripherals, and events can trigger DMA transfers.

The four DMA channels have individual configuration and control settings. This include source, destination, transfer
triggers, and transaction sizes. They have individual interrupt settings. Interrupt requests can be generated when a
transaction is complete or when the DMA controller detects an error on a DMA channel.
To allow for continuous transfers, two channels can be interlinked so that the second takes over the transfer when the
first is finished, and vice versa.

Properties
----------
* Four DMA channels with separate settings.
* Programmable channel priority.
* 1 byte to 16MB of data in a single transaction.
* Multiple addressing modes.
* Optional reload of source and destination addresses.
* Optional interrupt on end of transaction.
* Optional connection to CRC generator for CRC on DMA data.

Features
------------
* Initialization.

Applications
------------
* High speed data transfers with minimal CPU intervention.
* Direct data transfer between memories and peripherals.
* Direct data transfer between communication modules.

Dependencies
------------
* CLK for clock
* CPUINT/PMIC for Interrupt
* PORT for I/O Lines and Connections
* UPDI/PDI/JTAG for debug
* EVSYS for Event

Note
----
* ISR will be generated only when Global Interrupt checkbox and driver_isr Harness checkbox are enabled

Concurrency
-----------
N/A

Limitations
-----------
N/A

Knows issues and workarounds
----------------------------
N/A


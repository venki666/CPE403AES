TI 15.4-Stack Sensor Example
=======================


Example Summary
---------------

The sensor example application demonstrates how to implement a sensor network device using TI 15.4-Stack. TI 15.4-Stack based star network consists of two types of logical devices: the PAN-Coordinator and the network devices, e.g. the Collector and Sensor applications, respectively.

The PAN-Coordinator is the device that starts the network and allows other devices to join the network. The network devices join the network through and always communicate with the PAN-Coordinator.

The example applications provided in TI 15.4-Stack are developed for the CC13x0 Launchpad platform. In addition, the Linux example applications for the external host (AM335x + MAC Coprocessor) are located in the [**TI 15.4-Stack Gateway SDK**](http://www.ti.com/tool/ti-15.4-stack-gateway-linux-sdk).

> The project names for CC1310 and C1350 platforms are referred to as CC13x0. Replace x with either **1** or **5** depending on the specific wireless MCU being used.

Peripherals Exercised
---------------------

* `Board_LED0` - Set when the the sensor successfully connects to a PAN.
* `Board_LED1` - Toggled when the sensor sends measurement data to the collector.
* `Board_BUTTON0` - Press to disassociate from the PAN the sensor is currently connected to.
* `Board_BUTTON1` - If **AUTO_START** is not defined, Press to initialize the sensor application.

> If the **AUTO_START** symbol is defined in your application, then the application will automatically configure itself on startup.

> If the **AUTO_START** symbol is not defined pressing `Board_BUTTON1` will initialize the sensor application until the sensor has connected to a network.

> If `Board_BUTTON1` is held while power is applied to the Launchpad, NV Flash will be erased.

Resources & Jumper Settings
---------------------------
The following hardware is required to run TI 15.4-Stack Out of Box (OOB) example applications:
* Two [**CC13x0 Launchpad development kits**](http://www.ti.com/tool/launchxl-cc1310)
* Optional - Two [**LCD Boosterpack modules**](http://www.ti.com/tool/430boost-sharp96)

> If you're using an IDE (such as CCS or IAR), please refer to `Board.html` in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find `Board.html` in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

Example Usage
-------------
This example project implements a sensor end device: one of potentially many network devices in a PAN. This end device reads sensor information and sends it to the PAN-coordinator at a configured interval. This example assumes a second Launchpad is running the default collector application code to act as the PAN-coordinator.

The example output can either be viewed by connecting the optional LCD Boosterpack, or through the UART terminal.

* Open a serial session (e.g. [`PuTTY`](http://www.putty.org/ "PuTTY's
Homepage"), etc.) to the appropriate COM port with the following settings.

> The COM port can be determined via Device Manager in Windows or via
`ls /dev/tty*` in Linux.

* Upon example start, the connection will have the following settings:
```
    Baud-rate:     115200
    Data bits:          8
    Stop bits:          1
    Parity:          None
    Flow Control:    None
```
and initially display the following text on either the optional LCD or UART terminal.
```
    TI Sensor
    Waiting... (If AUTO_START is defined)
```

* Start the application by pressing `Board_BUTTON1` and sensor specific application information will be displayed, such as the devices current state.
```
    Starting...
    State Changed: 1
```

> The joining device state variable information can be seen in the Joining Device Logical Link Controller's header file (`jdllc.h`), within the `Jdllc_states_t` structure.

* Wait for the sensor device to join a network, after which the output will be updated with the channel number and device id of the sensor that was started. After joining the network `Board_LED0` will be set.
```
    Sensor Device
    State Changed: 3
    Started: 0x1
    Channel: 0
```

* After the sensor node has successfully joined the network, it receives a configuration request message from the collector application. The node then configures the time interval on how often to report the sensor data to the collector application, and how often to poll for buffered messages in case of sleepy devices. After receiving the configuration request message, the green LED (`Board_LED1`) toggles whenever the device sends the message.

> In order for the network device to join, it must have either the generic PAN Id (0xFFFF, default configuration) or the same PAN Id as the collector. These settings can be found in the application's `config.h` file.

Analyzing Network Traffic
-------------------------

TI 15.4-Stack provides the means to analyze over-the-air traffic by including a packet sniffer firmware image. With an additional CC13x0 Launchpad, users can set up a packet sniffer with the software provided in the SDK. More information about this can be found in the TI 15.4-Stack documentation under **Packet Sniffer**.


Viewing Device PER (Packet Error Rate)
--------------------------------------

PER is a simple value with the following equation
* PER = 100 * (Number of Failed Packets / (Number of Successful Packets + Number of Failed Packets))

This value can be used to judge how well the network doing.

If you would like to see the stats on the Number of Failed and Successful Packets then simply define **DISPLAY_PER_STATS**. This will add the code necessary to update and display the stats to the UART Display.

Application Design Details
--------------------------

See `config.h` and `features.h` for the default application settings. Some important settings in config.h include:

* `CONFIG_SECURE` - enables/disables MAC security. Default is **enabled**
* `CONFIG_FH_ENABLE` - enables/diables Frequency Hopping mode. Default is **diabled**
* `CONFIG_CHANNEL_MASK` - sets the active scanning channels mask. Default is **channels 0-3**
* `CONFIG_BEACON_ORDER` - sets beacon order for beacon/non-beacon mode. 0-14 are for beacon mode and 15 is for non-beacon mode. Default is **15** (non-beacon mode)
* `CONFIG_SUPERFRAME_ORDER` - sets superframe order for beacon/non-beacon mode. 0-14 are for beacon mode and 15 is for non-beacon mode. Default is **15** (non-beacon mode)

> The `features.h` file can be modified in order to disable certain features to **save code space**.

More information about the configuration and feature options can be found in the TI 15.4-Stack documentation under **Example Applications > Configuration Parameters**.

A detailed description of the application architecture can be found in your installation within the
TI-15.4 Stack Getting Started Guide's Application Overview section.

&lt;SDK_INSTALL_DIR&gt;/docs/ti154stack/ti154stack-getting-started-guide.html.

> For IAR users:
When using the CC13x0DK, the TI XDS110v3 USB Emulator must
be selected. For the CC13x0_LAUNCHXL, select TI XDS110 Emulator. In both cases,
select the cJTAG interface.

In order to build from flash, within the IAR Project options-> Build Actions
Update the "Pre-build command line" and change the "NO_ROM=0" to "NO_ROM=1".


## Example Summary

This example is intended to be a starting point for new development where
a minimal footprint is needed.

## Peripherals Exercised

* `Board_LED0` - Indicates that the board was initialized within `main()`

## Example Usage

* The example lights `Board_LED0` as part of the initialization in `main()`.
Then a heartBeat task toggles the LED at a rate determined by the `arg0`
parameter for the constructed Task instance in the .c file.

## Application Design Details

This examples is the same as the Empty example except many development
and debug features are disabled. For example:

* No Kernel Idle task
* No stack overflow checking
* No Logs or Asserts are enabled


> The ROM is being used in this example. This is controlled
> by the following lines in the *.cfg* file:

> ```
var ROM = xdc.useModule('ti.sysbios.rom.ROM');
    ROM.romName = ROM.CC2650;
```
> Since the kernel in the ROM is being used, there is no logging or assert
checking done by the kernel.

> For IAR users using any SensorTag(STK) Board, the XDS110 debugger must be
selected with the 4-wire JTAG connection within your projects' debugger
configuration.

## References

* For GNU and IAR users, please read the following website for details
  about enabling [semi-hosting](http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting)
  in order to view console output.

* Please refer to the __Memory Footprint Reduction__ section in the
TI-RTOS User Guide *spruhd4.pdf* for a complete and detailed list of the
differences between the empty minimal and empty projects.

# WUPS Plugin Playground

This repository is supposed to hold serveral small testing plugins for the [Wii U Plugin System](https://github.com/Maschell/WiiUPluginSystem).
 Most of the plugin are either really small (Padcon, NNU_Patcher) or are just testing / gathering information.

## List of plugins:

Here is a small overview of the plugins currently avaiable on this repository.

### memory info
Collects and logs some information about the current memory usage when pressing L+R+X+Y+A+B on the gamepad. Logging can be seen with the 
[UDPDebugReader](https://github.com/Maschell/WiiUPluginSystem/tree/master/tools/udp_debug_reader).

### nnu patcher
The nnu patcher patches the `NeedsNetworkUpdate` function to always reply with "false" to allows access to the EShop on lower firmwares.

### overlay test
The overlay test is small demo to show the overlay feature of the Wii U Plugin System. Press A,B,X and Y while gameplay and a text should appear.
 In the future the menu could hold configuration for currently running plugins.

### padcon
The padcon plugin allows you to turn off the screen of your gamepad to save energy.
It patches the `VPADRead` function which is responseable for reading the input of the Wii U gamepad. Whenever someone _presses the right analog stick_, the screen will turn off.

### vpad input logger
The vpad input logger logs serveral information about the current gamepad input.
Beside the pressed buttons and touched position on the touchscreen, the plugin also logs the value of the sensors (gyro, magnet etc.)
Logging can be seen with the [UDPDebugReader](https://github.com/Maschell/WiiUPluginSystem/tree/master/tools/udp_debug_reader).

## Building:

For building you need: 
- [wups](https://github.com/Maschell/WiiUPluginSystem)
- [wut](https://github.com/decaf-emu/wut)
- [libutilswut](https://github.com/Maschell/libutils/tree/wut) (WUT version) for common functions.

Some plugins may require more libraries to be installed. Checkout their Makefile for more information.
You could also take a look at the provided travis script and extract the needed building steps.
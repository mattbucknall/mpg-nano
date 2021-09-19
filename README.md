# MPG-Nano
Firmware and UCCNC macro for Arduino Nano based serial-over-USB interface for modified 4-axis Chinese
MPG pendant.
<p align="center"><img src="./pendant.jpg" width=150 /></p>

# Functions
The pendant LED flashes slowly when the axis select switch is in any position other than 'Off'. The step
size select switch selects between step sizes of 1 micron/step, 10 microns/step and 100 microns/step. The pendant's side switch selects 1mm/step 'rapid mode' when pressed (if wiring modification documented below has been made). The pendant's LED flashes fast when 1mm/step is selected. The Arduino Nano's User LED flashes continuously to indicate that the firmware is running.

## Compiling & Programming
To build the firmware, this project requires avr-gcc, avr-libc and avrdude to be correctly installed on
a host PC. Type `make program` to program the Nano (the Makefile for this project assumes that an AVR-ISP MkII programmer is being used).

The Makefile has only been tested in a Linux development environment. It may need modification to work in
Windows/OS X.

## Side Button Modification
The firmware supports an optional modification to the pendant's internal wiring such that the side button
acts as a x1000 'rapid mode' selector instead of a pendant enable button.

Modifications to the pendant's internal wiring are as follows:

1. Identify red wire linking side button to internal PCB. Unsolder wire from button.
2. Unsolder blue/black wire from E-Stop button.
3. Solder red wire (unsoldered in Step 1) to same pin as orange/black wire, on side button.
4. Solder blue/black wire (unsoldred in Step 2) to side button pin where red wire was originally soldered.
5. Add link between side button pin, where orange/black wire is soldred, and E-Stop button pin where blue/black wire was originally soldered.

<img src="./wiring-mod.jpg" width=400 />

## Connections to Arduino Nano
Assuming that the pendant has had its wiring modified to support 'rapid mode', it must be connected to the Arduino Nano's pins as follows:

| Wire Color   | Arduino Pin | Description   |
|--------------|-------------|---------------|
| Orange/Black | GND         | Switch Common |
| Gray         | D2          | x1 Select     |
| Gray/Black   | D3          | x10 Select    |
| Orange       | D4          | x100 Select   |
| Green/Black  | D5          | LED+          |
| Blue         | D6          | E-Stop        |
| Blue/Black   | D7          | Rapid Select  |
| Brown        | D8          | Z Select      |
| Brown/Black  | D9          | 4 Select      |
| Green        | A0          | Encoder A+    |
| Violet       | A1          | Encoder A-    |
| White        | A2          | Encoder B+    |
| Violet/Black | A3          | Encoder B-    |
| Yellow       | A4          | X Select      |
| Yellow/Black | A5          | Y Select      |
| Red          | +5V         | Encoder +5V   |
| Black        | GND         | Encoder GND   |
| White/Black  | GND         | LED-          |

If the pendant has not been modified then the Blue/Black wire must be connected to GND.

## Protocol

The serial protocol implemented by the firmware operates at 38400 baud with an 8-bit, no-parity, 1 stop
bit word format.

### Reset Command
Sending an upper-case `R` character to the Nano will reset the firmware's encoder pulse count to zero.
The firmware acknowledges the command by sending back `[R]` followed by a `CR` `LF` (carriage-return,
line-feed) sequence.

### Status Command
Sending an upper-case `S` character to the Nano will cause the firmware to return `[Sxxxxxx]` followed by
a `CR` `LF` sequence, where `xxxxxx` is a 6-digit/24-bit hexadecimal status word.

The status word is encoded as follows:

| Bit(s) | Description                                                                                                                                  |
|--------|----------------------------------------------------------------------------------------------------------------------------------------------|
| 23 - 8 | A 16-bit two's complement value indicating the change in encoder wheel pulse count since the last `S` or `R` command was received.           |
| 7 - 6  | Unused, always zero.                                                                                                                         |
| 5      | Set if E-Stop button is pressed.                                                                                                             |
| 4 - 3  | A 2-bit field indicating the selected step size:<br><br>0: x1<br>1: x10<br>2: x100<br>3: x1000 (rapid mode button held down)                 |
| 2 - 0  | A 3-bit field indicating which axis is selected:<br><br>0: Off<br>1: X<br>2: Y<br>3: Z<br>4: 4                                               |

### UCCNC Plugin
If you are using a genuine Arduino Nano, ensure you have the FTDI VCP driver installed
(https://ftdichip.com/drivers/vcp-drivers/). If you are using a CH340 based Arduino Nano clone,
ensure you have the CH340 driver installed
(https://learn.sparkfun.com/tutorials/how-to-install-ch340-drivers/all).

To install the MPG-Nano plugin, copy the DLL in this repository to UCCNC's Plugins folder, then enable
the plugin as described in UCCNC's manual ('Installing plugins'). Once the plugin is enabled and UCCNC has
been restarted, connect the Arduino Nano (running the MPG-Nano firmware) to your PC. Navigate to UCCNC's
'Plugins configuration' window and click the 'Configure' button for MPG-Nano, select the COM port that
Windows has assigned to the Nano, then check that 'Status' changes to 'OK'.

If you are not sure which COM port has been assigned to the Nano, go to Windows Device Manager, expand
the 'Ports' section in the device list, then unplug the Nano and plug it back in - You should see a COM
port disappear then reappear in the device list - This is the port assigned to the Nano.

Source code for the UCCNC MPG-Nano plugin can be found here:
https://github.com/mattbucknall/mpg-nano-plugin

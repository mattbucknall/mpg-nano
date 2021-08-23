# MPG-Nano
Firmware and UCCNC macro for Arduino Nano based serial-over-USB interface for modified 4-axis Chinese MPG pendant.

# Functions
The pendant LED flashes slowly when the axis select switch is in any position other than 'Off'. The step size select switch selects between step sizes of 1 micron/step, 10 microns/step and 100 microns/step. The pendant's side switch selects 1mm/step when pressed (requires wiring modification documented below). The pendant's LED flashes fast when 1mm/step is selected. The Arduino Nano's User LED flashes continuously to indicate that the firmware is running.

## Compiling & Programming
To build the firmware, this project requires avr-gcc, avr-libc and avrdude to be correctly installed. To program the Arduino Nano, it must already be flashed with the Arduino bootloader. Attach the Nano to a USB port on your PC and execute `make program` from within the project directory. The Makefile assumes that the Nano presents itself as `/dev/ttyUSB0` when plugged into a USB port. Modify the `ARDUINO_SERIAL` variable in the Makefile if this is not the case.

The Makefile has only been tested in a Linux environment. It may need modification to work in Windows.

## Side Button Modification
The firmware assumes that the pendant's internal wiring has been modified so that the side button acts as a x1000 'rapid mode' selector instead of an enable button.

Modifications to the pendant's internal wiring are as follows.

1. Unsolder red wire linking side button and internal PCB from button.
2. Unsolder blue/black wire from E-Stop button.
3. Solder red wire to same pin, on side button, as the orange/black wire is soldered.
4. Solder blue/black wire to side button pin where red wire was originally soldered.
5. Add link between side button pin where orange/black wire is soldred to E-Stop button pin where blue/black wire was originally soldered.

## Connections to Arduino Nano
The pendant's wires must be connected to the Arduino Nano's pins as follows:

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

In the author's implementation, the pendant cable was terminated with a 25-pin D-Type connector which mated with a connector mounted on a metal enclosure housing the Nano.

## TODO
Develop and document UCCNC macro loop.

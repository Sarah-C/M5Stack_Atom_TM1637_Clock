# M5Stack Atom: TM1637 Clock
A simple TM1637 of a 12/24 hour clock with seconds display.

It uses a simple delay() to keep time, so it's not all that accurate - but shows a few tricks with the TM1637 clock display, like creating a few new characters to display, and clearing only portions of the display.
Interestingly the flashing : symbol is the same as the "decimal point" of other displays - in that it's lumped in with the digit directly to its left. In this case the flag is bit bit 7 on the second character along.

It can be run on pritty much any M5Stack device, just by changing the include at the top.

The version of the display I have coincidentally plugged straight into the 4 pins of my Atom without needing wiring. GND, VCC, DIO, CLK.

<img src="https://user-images.githubusercontent.com/1586332/133896248-3e98f505-e8f6-4274-b8ff-e36834d17275.png" width="600">


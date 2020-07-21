## Hardware
This code works on this (retired) Sparkfun Geiger counter board -> https://www.sparkfun.com/products/retired/11345

## Setup
Plug the board into your USB port and open this project in the Arduinio IDE. In the IDE select the serial port assigned to the board. Use board type “Arduino Pro or Pro Mini" and processor ATmega328 (3.3V, 8 MHZ). 
## Usage
 After a short period you will notice the "counter" light on the board start to turn on and off. This is the only physical indicator the sketch is working. Set the serial port speed to 9600 and start the serial monitor then wait. Around 40ish minutes later there will print a JSON snippet with a 128-bit number printed in binary as well as hex and the time in milliseconds it took to generate.

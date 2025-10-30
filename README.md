# MCP4151 Arduino Library
Control the digital potentiometer MCP4151 from your Arduino.<br>

## Compatibility to Arduino Hardware Variants
Since this library only uses three general purpose digital I/O pins, at the moment there is no compatibility issue foreseen with any model of Arduino. If you notice some incompatibility however, feedback is welcome.

## Minimal Library Usage Example

```arduino
#include <MCP4151.h>

MCP4151 mcp4151(2,3,4);

void setup() {
    mcp4151.begin()
    mcp4151.setWiper(128); // set potentiometer mid-point
}

void loop() {}
```

## Electrical Circuitry Setup
The above code only will be of use if you first hook up the circuitry correctly, which is now explained:<br>
### Deviation from the datasheet
The MCP4151 is no classical SPI device with 4-wire SPI. Instead, 3-wire SPI has been chosen by the designers. The datasheet proposes interfacing 4 pins of the master/controller side 4-wire SPI device (Arduino) with 3 pins of the 3-wire MCP4151. However, do not do this if you use this library. This lib implements real 3-wire SPI also on Arduino side. So you only need 3 wires sticking to your Arduino. Yay! Read on.
### How to hook the whole thing up
As seen in the introductory code snippet:<br>
```arduino
MCP4151 mcp4151(2,3,4)
``` 
There are three pins of the Arduino chosen at random/your choice:
* **2** is chosen as **CS** pin in the example code
* **3** is chosen as **SCK** pin in the example code
* **4** is chosen as **SDIO** pin in the example code

You can choose **any three pins** of the Arduino, **as long as** they can be digital outputs aka ```pinMode(PIN, OUTPUT)``` and ```digitalWrite(PIN)``` work normally on your chosen Arduino pins.<br>
You then just connect the pins of Arduino with the pins with corresponding role on the MCP4151 (CS to CS, SCK to SCK), **except that** the chosen pin for SDIO transfer (pin 4 in the code example above) MUST be connected via a resistor to the SDIO pin of the MCP4151. For example I chose 330Ohm and it worked. 1kOhm did not work for me by the way.<br>
This section has been talking about the pins that take part in data transfer. On the MCP4151 these are in my IC variant these are pins 1 throught 3 (CS, SCK, SDIO). Next section talks about the three pins that comprise the electrical potentiometer interface (pins 5 through 7 in my variant).


## Potentiometer Value Range
The example above sets the potentiometer wiper to mid-value: ```mcp4151.setWiper(128)```.<br>
Generally the wiper can be set to values from 0 to 256.<br>
The wiper is also known as pin P0W or pin 6 (look into your datasheet pinout picture). <br>
If we compare the digital potentiometer to an analog potentiometer where the wiper can be adjusted from one side of a resistance slider to the other side. Then a wiper value of 0 in this code library means that the wiper/slider is near P0B, while a wiper/slider value of 256 means the wiper/slider is near P0A. This is remembered from my measurements - please make sure to verify yourself to be safe.<br>
The included Arduino example sketch **./examples/UserPotentiometerValueInput** accessible in Arduino IDE via 
```
Files --> Examples --> MCP4151 --> UserPotentiometerValueInput
```
can be used to manually set values for the wiper and analyse potentiometer/voltage divider behavior.<br>

# Installation Guide

Click here in Github on the green **<>Code** button and then **Download ZIP**.
```
<>Code --> Download ZIP (here in GitHub)
```
Then in the **Arduino IDE** click:
```
Sketch --> Include Library --> Add .ZIP Library...
```
And select the ZIP file you downloaded in the first step. The Arduino IDE then quickly will inform you about successful installation.<br>
Then if you have a sketch that you want to use the library in go to the Arduino IDE menu bar and click:
```
Sketch --> Include Library --> MCP4151
```
and use the functionality of this library in your sketch. Happy coding!

## Whats Next
Arduino Library Manager inclusion is coming soon.<br>
Support for mutliple MCP4151 devices sharing the same SCK and SDIO pins on the Arduino are an idea.






# Arduino Parallel Thermal Printer library
An elegant and efficient Arduino library for controlling thermal printers (POS) that got a [parallel](https://en.wikipedia.org/wiki/Parallel_port) or a [Centronix port](https://en.wikipedia.org/wiki/Parallel_port#Centronics). The library is based on the Adafruit serial thermal printer library.


### Serial printer mode
The **Serial_printer** example sketch lets you print text from the serial monitor. Note that you need a "real" serial terminal program that supports that supports sending ASCII control characters such as *Ctrl + B*, *Ctrl + U* and *Ctrl + X* in order to bold, underline and cut the paper. Recommended Serial terminals are [Tera Term](https://ttssh2.osdn.jp/index.html.en) (Windows) and [CoolTerm](http://freeware.the-meiers.org) (Mac).


## Supported hardware
Any Arduino compatible microcontroller with hardware serial and at least 11 IO pins.

## Installation
Click on the "Download ZIP" button. Exctract the ZIP file, and move the extracted folder to the location "**~/Documents/Arduino/libraries**". Create the "libraries" folder if it doesn't exist.
Open Arduino IDE, and a new library called "**Parallel Thermal printer**" will show up under the "**examples**" menu.


## Constructors
### AVR only
Use this constructor if you're using a single physical port for your parallel data bits (AVR microcontrollers only):
```C++
Printer Thermal(&PORTC, strobePin, busyPin, selectPin);

// Identical:
Printer Thermal(&PORTC, strobePin, busyPin, selectPin, MSBFIRST);
```

If the data bits are flipped:
```C++
Printer Thermal(&PORTC, strobePin, busyPin, selectPin, LSBFIRST);
```

### All Arduino compatible microcontrollers
Use this constructor if you're using separate pins for your parallel data bits (All arduino compatible microcontrollers):
```C++
Printer Thermal(/*bit 7*/ dataPin7, /*bit 6*/ dataPin6, /*bit 5*/ dataPin5, /*bit 4*/ dataPin4, /*bit 3*/ dataPin3, /*bit 2*/ dataPin2, /*bit 1*/ dataPin1, /*bit 0*/ dataPin0, strobePin, busyPin, selectPin);
// Identical
Printer Thermal(/*bit 7*/ dataPin7, /*bit 6*/ dataPin6, /*bit 5*/ dataPin5, /*bit 4*/ dataPin4, /*bit 3*/ dataPin3, /*bit 2*/ dataPin2, /*bit 1*/ dataPin1, /*bit 0*/ dataPin0, strobePin, busyPin, selectPin, MSBFIRST);
```

If the data bits are flipped:
```C++
Printer Thermal(/*bit 0*/ dataPin0, /*bit 1*/ dataPin1, /*bit 2*/ dataPin2, /*bit 3*/ dataPin3, /*bit 4*/ dataPin4, /*bit 5*/ dataPin5, /*bit 6*/ dataPin6, /*bit 7*/ dataPin7, strobePin, busyPin, selectPin, LSBFIRST);
```


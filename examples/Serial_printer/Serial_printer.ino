/*
  These commands will NOT work with the Arduino serial
  monitor. You'll need to use a dedicated Terminal program
  such as Tera Term or CoolTerm.
  
  Bold           ->  Ctrl + b    -> ASCII char no. 2
  Clear          ->  Ctrl + c    -> ASCII char no. 3
  Invert text    ->  Ctrl + f    -> ASCII char no. 6
  Draw line      ->  Ctrl + l    -> ASCII char no. 12
  Font ++        ->  Ctrl + m    -> ASCII char no. 13
  Font --        ->  Ctrl + n    -> ASCII char no. 14
  Strike through ->  Ctrl + s    -> ASCII char no. 19
  Cut paper      ->  Ctrl + x    -> ASCII char no. 24
*/

#include "Printer.h"


// Parallel data pins
uint8_t dataBit7 = 13
uint8_t dataBit6 = 12
uint8_t dataBit5 = 11
uint8_t dataBit4 = 10
uint8_t dataBit3 = 9
uint8_t dataBit2 = 8
uint8_t dataBit1 = 7
uint8_t dataBit0 = 6

// Control pins
uint8_t strobe = 5;
uint8_t busy = 4;
uint8_t select = 3;


/**** Constructor where pin 6 - 13 is used to send the data ****/
Printer Thermal(dataBit7, dataBit6, dataBit5, dataBit4, dataBit3, dataBit2, dataBit1, dataBit0, strobe, busy, select, MSBFIRST);

/**** Constructor where pin 6 - 13 is used to send the data, but the pins are flipped ****/
//Printer Thermal(dataBit0, dataBit1, dataBit2, dataBit3, dataBit4, dataBit4, dataBit6, dataBit7, strobe, busy, select, LSBFIRST);


/**** Constructor where PORTC is used to send the data. Remember to set the port as output in the setup()****/
//Printer thermal(&PORTC, strobe, busy, select);

/**** Constructor where PORTC is used to send the data and the bit order are flipped. Remember to set the port as output in the setup() ****/
//Printer thermal(&PORTC, strobe, busy, select, LSBFIRST);



void setup()
{
 //DDRC = 0xFF; // Set port C as output

  // Start the printer and the serial port
  Thermal.begin(&Serial, 19200); // Serial object, baudrate
}



void loop() 
{
  // Receive the serial data and output it on the printer
  Thermal.getSerialData();
}

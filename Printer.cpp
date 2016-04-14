#include "Printer.h"
#include "Arduino.h"

//----- line 42 font 1
//----- line 42 font 2
//----- line 21 font 3
//----- line 10 font 4

// Constructor where a port are passed
Printer::Printer(volatile uint8_t *port, uint8_t strobe, uint8_t busy, uint8_t select, uint8_t bitOrder)
{
  // Store objects and variables
  _Serial = NULL;
  _port = port;
  portMode = true;
  
  _strobe = strobe;
  _busy = busy;
  _bitOrder = bitOrder;

  // Set the correct pin modes
  pinMode(_strobe, OUTPUT);
  pinMode(_busy, INPUT); 
  pinMode(select, OUTPUT);

  // Enable automatic linefeed if a string is too long for the paper to fit
  digitalWrite(select, HIGH);   
}

// Alternative constructor where 8 individual pins are passe
Printer::Printer(uint8_t bit7, uint8_t bit6, uint8_t bit5, uint8_t bit4, uint8_t bit3, uint8_t bit2, uint8_t bit1, uint8_t bit0, uint8_t strobe, uint8_t busy, uint8_t select, uint8_t bitOrder)
{
  // Store objects and variables
  _Serial = NULL;
  _port = NULL;
  portMode = false;
  
  _dataBit7 = bit7;
  _dataBit6 = bit6;
  _dataBit5 = bit5;
  _dataBit4 = bit4;
  _dataBit3 = bit3;
  _dataBit2 = bit2;
  _dataBit1 = bit1;
  _dataBit0 = bit0;
  _strobe = strobe;
  _busy = busy;
  _bitOrder = bitOrder;

  // Set the correct pin modes
  pinMode(_dataBit7, OUTPUT);
  pinMode(_dataBit6, OUTPUT);
  pinMode(_dataBit5, OUTPUT);
  pinMode(_dataBit4, OUTPUT);
  pinMode(_dataBit3, OUTPUT);
  pinMode(_dataBit2, OUTPUT);
  pinMode(_dataBit1, OUTPUT);
  pinMode(_dataBit0, OUTPUT);
  pinMode(_strobe, OUTPUT);
  pinMode(_busy, INPUT); 
  pinMode(select, OUTPUT);

  //Enable automatic linefeed if a string is too long for the paper to fit
  digitalWrite(select, HIGH);
}


//Begin method
void Printer::begin(HardwareSerial *serialPort, uint32_t baudrate)
{
  // Create a copy of the passed Serial object
  _Serial = serialPort;
  _Serial->begin(baudrate);

  //Reset all printer settings
  reset();
}


void Printer::getSerialData() 
{
  while(_Serial->available()) 
  {
    // Pass the 8-bit data to the write method
    write(_Serial->read());
  }
}


/* Receives a character, and decides which characters
 to print, and which will change the printer settings.
 This method is ONLY used for for receiving data from
 Serial! */
void Printer::write(uint8_t character)
{
  // Check if the received character was a command or
  // a printable ASCII character
  if (!command(character))
  {
    
    // Add an extra line if the paper have been cut
    if(paperCut == true)
    {
      writeByte(ASCII_NEWLINE);
      paperCut = false;
    }

    // Convert from UTF-8 to ASCII if necessary, and set
    // the byte on the parallel port
    writeByte(utf8ascii(character)); // Convert from UTF-8 to ASCII 

    // Output what control characters was sent
    if(character < 32 && character != ASCII_NEWLINE)
      _Serial->println("[Ctrl char. " + (String)character + "]");
    else
      _Serial->print((char)character);
  }
}


// Receives a string and prints it
void Printer::println(String text)
{
  for(uint16_t i = 0; i < text.length(); i++)
  {
    writeByte(text[i]);
  }
  writeByte(ASCII_NEWLINE);
}


// Handles all the control characters sent over serial
bool Printer::command(uint8_t character)
{
  switch(character)
  {
    // Ctrl + B
    case ASCII_BOLD:
      if(bold == false)
      {
        boldOn();
        bold = true;
        _Serial->println(F("[Bold selected]"));
      }
      else
      {
        boldOff();
        bold = false;
        _Serial->println(F("[Bold cleared]"));
      }
      return true;


    // Ctrl + C
    case ASCII_CLEAR:
      reset();
      _Serial->println(F("[All settings cleared]"));     
      return true;


    // Ctrl + F
    case ASCII_INVERT:
      if(inverted == false)
        {
          inverseOn();
          inverted = true;
          _Serial->println(F("[Inverted text selected]"));
        }
        else
        {
          inverseOff();
          inverted = false;
          _Serial->println(F("[Inverted text cleared]"));
        }
        return true;


    // Ctrl + L
    case ASCII_LINE:
      if(fontSize == 1 || fontSize == 2)
        println(F("------------------------------------------"));
      else if(fontSize == 3)
        println(F("---------------------"));
      else if(fontSize == 4)
        println(F("----------"));  
      _Serial->println(F("[Line printed]"));  
      return true;  
      
      
    case ASCII_STRIKE:
      if(strike == false)
      {
        strikeOn();
        _Serial->println(F("[Strike through selected]"));
        strike = true;
      }
      else
      {
        strikeOff();
        _Serial->println(F("[Strike through cleared]"));
        strike = false;
      }

    // Ctrl + X
    case ASCII_CUT:
      cut();
      _Serial->println(F("[Paper cut]"));
      paperCut = true;
      return true;

    // Ctrl + U
    case ASCII_UNDERLINE: 
      if(underline == false)
      {
        underlineOn(1);
        underline = true;
        _Serial->println(F("[Underline selected]"));
      }
      else
      {
        underlineOff();
        underline = false;
        _Serial->println(F("[Underline cleared]"));
      }
      return true;

    // Ctrl + N
    case ASCII_DECREASEFONT:
      if(fontSize == 1)
        _Serial->println(F("[Smallest font allready selected!]"));
      else
      {     
        switch(fontSize)
        {
          case 2:
            fontSize = 1;
            setSize(fontSize);
            _Serial->println(F("[Default font selected]"));           
            break;
          case 3:
            fontSize = 2;
            setSize(fontSize);
            _Serial->println(F("[Font size 2 selected]"));
            break;
          case 4:
            fontSize = 3;
            setSize(fontSize); 
            _Serial->println(F("[Font size 3 selected]")); 
            break;
        }

      }
      return true;

    // Ctrl + M
    case ASCII_INCREASEFONT:
      if(fontSize == 5)
        _Serial->println(F("[Largest font allready selected!]"));
      else
      {
        switch(fontSize)
        {
          case 1:
            fontSize = 2;
            setSize(fontSize);
            _Serial->println(F("[Font size 2 selected]"));
            break;
          case 2:
            fontSize = 3;
            setSize(fontSize);
            _Serial->println(F("[Font size 3 selected]"));
            break;
          case 3:
            fontSize = 4;
            setSize(fontSize); 
            _Serial->println(F("[Font size 4 selected]")); 
            break;  
        }
      }
      return true;
       
    default:
      return false;  
  }
}


void Printer::writeByte(uint8_t character)
{
  //If LSBFIRST is selected, flip the byte
  if(_bitOrder == LSBFIRST)
  {
   character = ((character >> 1) & 0x55) | ((character << 1) & 0xaa); 
   character = ((character >> 2) & 0x33) | ((character << 2) & 0xcc); 
   character = ((character >> 4) & 0x0f) | ((character << 4) & 0xf0);
  }

  // If a port is used for the parallel data
  if(portMode == true)
  {
    // Set the bits
    *_port = character;

    // Latch the bits
    digitalWrite(_strobe, LOW);
    delayMicroseconds(300);
    digitalWrite(_strobe, HIGH);

    // Wait for the printer to finish
    while(digitalRead(_busy) == HIGH);
  }

  // Else if each data pin is individually selected
  else
  {
    // Set the bits
    digitalWrite(_dataBit7, bitRead(character, 7));
    digitalWrite(_dataBit6, bitRead(character, 6));
    digitalWrite(_dataBit5, bitRead(character, 5));
    digitalWrite(_dataBit4, bitRead(character, 4));
    digitalWrite(_dataBit3, bitRead(character, 3));
    digitalWrite(_dataBit2, bitRead(character, 2));
    digitalWrite(_dataBit1, bitRead(character, 1));
    digitalWrite(_dataBit0, bitRead(character, 0));

    // Latch the bits
    digitalWrite(_strobe, LOW);
    delayMicroseconds(300);
    digitalWrite(_strobe, HIGH);

    // Wait for the printer to finish
    while(digitalRead(_busy) == HIGH);
  }
}


uint8_t Printer::utf8ascii(uint8_t data) 
{
  if (data < 128) // Standard ASCII-set 0..127 handling
  {
    previousCharacter = 0;
    return data;
  }

  byte last = previousCharacter; // get last char
  previousCharacter = data;      // remember actual character

  switch (last)  // conversion depnding on first UTF8-character
  { 
    case 0xC2: return data; break;
    case 0xC3: return (data | 0xC0); break;
    case 0x82: if(data == 0xAC) return 0x80;  // special case Euro-symbol
  }

  return 0; // otherwise: return zero, if character has to be ignored
}


void Printer::setSize(uint8_t value)
{
  uint8_t size;

  switch(value)
  {
    default:
      size = 0x00;
      fontSize = 1;
      break;
      
    case 2:
      size = 0x01;
      fontSize = 2;
      break;
      
    case 3:
      size = 0x11;
      fontSize = 3;
      break;
      
    case 4:
      size = 0x32;
      fontSize = 4;
      break;
  }
      //Set font
      writeByte(ASCII_GS);
      writeByte('!');
      writeByte(size);
      writeByte(ASCII_NEWLINE);      
}


void Printer::setPrintMode(uint8_t mask) 
{
  printMode |= mask;
  writePrintMode();
}


void Printer::unsetPrintMode(uint8_t mask) 
{
  printMode &= ~mask;
  writePrintMode();
}


void Printer::writePrintMode() 
{
  writeByte(ASCII_ESC);
  writeByte('!');
  writeByte(printMode);
}


void Printer::normal() 
{
  printMode = 0;
  writePrintMode();
}


void Printer::inverseOn()
{
  writeByte(ASCII_GS);
  writeByte('B');
  writeByte(1);
}


void Printer::inverseOff()
{
  writeByte(ASCII_GS);
  writeByte('B');
  writeByte(0);
}


void Printer::upsideDownOn()
{
  setPrintMode(UPDOWN_MASK);
}


void Printer::upsideDownOff()
{
  unsetPrintMode(UPDOWN_MASK);
}


void Printer::doubleHeightOn()
{
  setPrintMode(DOUBLE_HEIGHT_MASK);
}


void Printer::doubleHeightOff()
{
  unsetPrintMode(DOUBLE_HEIGHT_MASK);
}


void Printer::doubleWidthOn()
{
  setPrintMode(DOUBLE_WIDTH_MASK);
}


void Printer::doubleWidthOff()
{
  unsetPrintMode(DOUBLE_WIDTH_MASK);
}


void Printer::strikeOn()
{
  setPrintMode(STRIKE_MASK);
}


void Printer::strikeOff()
{
  unsetPrintMode(STRIKE_MASK);
}


void Printer::boldOn()
{
  setPrintMode(BOLD_MASK);
}


void Printer::boldOff()
{
  unsetPrintMode(BOLD_MASK);
}


void Printer::justify(char value)
{
  uint8_t pos = 0; 

  // toupper -> convert to upper case if necessary
  switch(toupper(value)) 
  {
    case 'L':
      pos = 0;
      break;
    case 'C':
      pos = 1;
      break;
    case 'R':
      pos = 2;
      break;
  }

  writeByte(ASCII_ESC);
  writeByte('a');
  writeByte(pos);
}


// Feeds by the specified number of lines
void Printer::feed(uint8_t x) 
{
  writeByte(ASCII_ESC);
  writeByte('d');
  writeByte(x);
  writeByte(ASCII_NEWLINE);
}


// Feeds by the specified number of individual pixel rows
void Printer::feedRows(uint8_t rows) 
{
  writeByte(ASCII_ESC);
  writeByte('J');
  writeByte(rows);
  writeByte(ASCII_NEWLINE);
}


// Print and return to standard mode
void Printer::flush()
{
  writeByte(ASCII_FF);
}


// 0 - no underline
// 1 - normal underline
// 2 - thick underline
void Printer::underlineOn(uint8_t thickness)
{
  if(thickness > 2) thickness = 2;
    
  writeByte(ASCII_ESC);
  writeByte('-');
  writeByte(thickness);  
}


void Printer::underlineOff() 
{
  writeByte(ASCII_ESC);
  writeByte('-');
  writeByte(0);
}


void Printer::setLineHeight(uint8_t val)
{
  if(val < 24) val = 24;
  writeByte(ASCII_ESC);
  writeByte('3');
  writeByte(val);
}


void Printer::setCharSpacing(uint8_t space)
{
  writeByte(ASCII_ESC);
  writeByte(' ');
  writeByte(space);
}


// Selects alt symbols for 'upper' ASCII values 0x80-0xFF
void Printer::setCodePage(uint8_t val) 
{
  if(val > 47) val = 47;
  writeByte(ASCII_ESC);
  writeByte('t');
  writeByte(val);
}


// Alters some chars in ASCII 0x23-0x7E range
void Printer::setCharset(uint8_t val) 
{
  if(val > 15) val = 15;
  writeByte(ASCII_ESC);
  writeByte('R');
  writeByte(val);
}


void Printer::cut()
{
  for(uint8_t i = 0; i < 7; i++)
    writeByte(ASCII_NEWLINE);
        
  writeByte(ASCII_ESC);
  writeByte('i');
  writeByte(ASCII_NEWLINE);
  paperCut = true;
}


void Printer::reset() 
{
  writeByte(ASCII_ESC);
  writeByte('@'); 
  writeByte(ASCII_NEWLINE);
  
  barcodeHeight = 50;

  writeByte(ASCII_ESC);
  writeByte('D'); // Set tab stops...
  writeByte(4);
  writeByte(8);
  writeByte(12);
  writeByte(16); // ...every 4 columns,
  writeByte(20); 
  writeByte(24);
  writeByte(28);
  writeByte(0); // 0 marks end-of-list.
}


// Default is 50
void Printer::setBarcodeHeight(uint8_t val) 
{ 
  if(val < 1) val = 1;
  barcodeHeight = val;
  writeByte(ASCII_GS);
  writeByte('h');
  writeByte(val);
}

void Printer::printBarcode(char *text, uint8_t type) 
{
  writeByte(ASCII_GS);
  writeByte('H');
  writeByte(2);    // Print label below barcode
  
  writeByte(ASCII_GS);
  writeByte('w');
  writeByte(3);    // Barcode width 3 (0.375/1.0mm thin/thick)

  writeByte(ASCII_GS);
  writeByte('k');
  writeByte(type); // Barcode type (listed in .h file)

  int len = strlen(text);
  if(len > 255) len = 255;
  writeByte(len);   
  
  // Write length byte
  for(uint8_t i=0; i<len; i++) 
    writeByte(text[i]); // Write string sans NUL

  writeByte(ASCII_NEWLINE);
}

void Printer::printBitmap(uint16_t w, uint16_t h, const uint8_t *bitmap, bool fromProgMem) {

  uint16_t rowBytes, x, y, i;

  rowBytes = (w + 7) / 8; // Round up to next byte boundary

  writeByte(ASCII_GS);
  writeByte('v');
  writeByte('0');
  writeByte(0);
  writeByte(rowBytes % 256);
  writeByte(rowBytes / 256);
  writeByte(h % 256);
  writeByte(h / 256);
  i = 0;
  for(y=0; y < h; y++) {
    for(x=0; x < rowBytes; x++, i++) {
      writeByte(fromProgMem ? pgm_read_byte(bitmap + i) : *(bitmap+i));
    }
  }
  writeByte(ASCII_NEWLINE);
}





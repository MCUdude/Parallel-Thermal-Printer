 /*------------ Parallel Thermal Printer library ----------------|
 |                                                               |
 | Created April 2016 by MCUdude, https://github.com/MCUdude     |
 | Released to public domain                                     |
 |                                                               |
 | This library lets you interface with a POS thermal printer    |
 | with a parallel interface using an Arduino or an Arduino      |
 | compatible microcontroller.                                   |
 |                                                               |
 |--------------------------------------------------------------*/


#include "Arduino.h"

#ifndef Printer_h
#define Printer_h

// Serial commands
#define ASCII_BOLD 2
#define ASCII_CLEAR 3
#define ASCII_INVERT 6
#define ASCII_NEWLINE 10
#define ASCII_FF 12
#define ASCII_LINE 12
#define ASCII_INCREASEFONT 13
#define ASCII_DECREASEFONT 14
#define ASCII_STRIKE 19
#define ASCII_UNDERLINE 21
#define ASCII_CUT 24
#define ASCII_ESC 27
#define ASCII_GS 29

#define INVERSE_MASK       (1 << 1) 
#define UPDOWN_MASK        (1 << 2)
#define BOLD_MASK          (1 << 3)
#define DOUBLE_HEIGHT_MASK (1 << 4)
#define DOUBLE_WIDTH_MASK  (1 << 5)
#define STRIKE_MASK        (1 << 6)

// Character and barcode definitions
#define UPC_A   65
#define UPC_E   66
#define EAN13   67
#define EAN8    68
#define CODE39  69
#define ITF     70
#define CODABAR 71
#define CODE93  72
#define CODE128 73
#define CHARSET_USA           0
#define CHARSET_FRANCE        1
#define CHARSET_GERMANY       2
#define CHARSET_UK            3
#define CHARSET_DENMARK1      4
#define CHARSET_SWEDEN        5
#define CHARSET_ITALY         6
#define CHARSET_SPAIN1        7
#define CHARSET_JAPAN         8
#define CHARSET_NORWAY        9
#define CHARSET_DENMARK2     10
#define CHARSET_SPAIN2       11
#define CHARSET_LATINAMERICA 12
#define CHARSET_KOREA        13
#define CHARSET_SLOVENIA     14
#define CHARSET_CROATIA      14
#define CHARSET_CHINA        15
#define CODEPAGE_CP437        0 // USA, Standard Europe
#define CODEPAGE_KATAKANA     1
#define CODEPAGE_CP850        2 // Multilingual
#define CODEPAGE_CP860        3 // Portuguese
#define CODEPAGE_CP863        4 // Canadian-French
#define CODEPAGE_CP865        5 // Nordic
#define CODEPAGE_WCP1251      6 // Cyrillic
#define CODEPAGE_CP866        7 // Cyrillic #2
#define CODEPAGE_MIK          8 // Cyrillic/Bulgarian
#define CODEPAGE_CP755        9 // East Europe, Latvian 2
#define CODEPAGE_IRAN        10
#define CODEPAGE_CP862       15 // Hebrew
#define CODEPAGE_WCP1252     16 // Latin 1
#define CODEPAGE_WCP1253     17 // Greek
#define CODEPAGE_CP852       18 // Latin 2
#define CODEPAGE_CP858       19 // Multilingual Latin 1 + Euro
#define CODEPAGE_IRAN2       20
#define CODEPAGE_LATVIAN     21
#define CODEPAGE_CP864       22 // Arabic
#define CODEPAGE_ISO_8859_1  23 // West Europe
#define CODEPAGE_CP737       24 // Greek
#define CODEPAGE_WCP1257     25 // Baltic
#define CODEPAGE_THAI        26
#define CODEPAGE_CP720       27 // Arabic
#define CODEPAGE_CP855       28
#define CODEPAGE_CP857       29 // Turkish
#define CODEPAGE_WCP1250     30 // Central Europe
#define CODEPAGE_CP775       31
#define CODEPAGE_WCP1254     32 // Turkish
#define CODEPAGE_WCP1255     33 // Hebrew
#define CODEPAGE_WCP1256     34 // Arabic
#define CODEPAGE_WCP1258     35 // Vietnam
#define CODEPAGE_ISO_8859_2  36 // Latin 2
#define CODEPAGE_ISO_8859_3  37 // Latin 3
#define CODEPAGE_ISO_8859_4  38 // Baltic
#define CODEPAGE_ISO_8859_5  39 // Cyrillic
#define CODEPAGE_ISO_8859_6  40 // Arabic
#define CODEPAGE_ISO_8859_7  41 // Greek
#define CODEPAGE_ISO_8859_8  42 // Hebrew
#define CODEPAGE_ISO_8859_9  43 // Turkish
#define CODEPAGE_ISO_8859_15 44 // Latin 3
#define CODEPAGE_THAI2       45
#define CODEPAGE_CP856       46
#define CODEPAGE_CP874       47



class Printer
{
  public:    
    // Public methods

    // Constructor where a port is used to set the data bits
    Printer(volatile uint8_t *port, uint8_t strobe, uint8_t busy, uint8_t select, uint8_t bitOrder = MSBFIRST);

    // Constructor where the data pins can be individually selected
    Printer(uint8_t bit7, uint8_t bit6, uint8_t bit5, uint8_t bit4, uint8_t bit3, uint8_t bit2, uint8_t bit1, uint8_t bit0, uint8_t strobe, uint8_t busy, uint8_t select, uint8_t bitOrder = MSBFIRST);

    // Begin method where the parameters are optional. Serial 0 with baudrate 19200 will automatically be selected if nothing is specified
    void begin(HardwareSerial *serialPort = &Serial, uint32_t baudrate = 19200);

    // This method will have to run constantly if you want to use the serial monitor to send data
    void getSerialData();

    // Will write a character
    void write(uint8_t character);

    // Will write a string
    void println(String text);

    // Set the font size, 1 - 4. Default is 1
    void setSize(uint8_t value);
    void normal();
    void inverseOn();
    void inverseOff();
    void upsideDownOn();
    void upsideDownOff();
    void doubleHeightOn();
    void doubleHeightOff();
    void doubleWidthOn();
    void doubleWidthOff();
    void strikeOn();
    void strikeOff();
    void boldOn();
    void boldOff();
    void justify(char value);
    void feed(uint8_t x = 1);
    void feedRows(uint8_t rows);
    void flush();
    void underlineOn(uint8_t weight = 1);
    void underlineOff();
    void setLineHeight(uint8_t val = 30);
    void setCharSpacing(uint8_t spacing = 0);
    void setCodePage(uint8_t val = 0);
    void setCharset(uint8_t val = 0);
    
    void cut();
    void reset();
    void setBarcodeHeight(uint8_t val=50);
    void printBarcode(char *text, uint8_t type);  
    void printBitmap(uint16_t w, uint16_t h, const uint8_t *bitmap, bool fromProgMem = true);
    
    
    
    

        
  private:
    // Private methods
    void writeByte(uint8_t character);
    bool command(uint8_t character);
    uint8_t utf8ascii(uint8_t data);
    void setPrintMode(uint8_t mask);
    void unsetPrintMode(uint8_t mask);
    void writePrintMode();

    // Private HW and object related data    
    volatile uint8_t *_port;
    HardwareSerial *_Serial;
    bool port_mode = false;
    bool pin_mode = false;


    // Hardware relatred
    uint8_t _dataBit7;
    uint8_t _dataBit6;
    uint8_t _dataBit5;
    uint8_t _dataBit4;
    uint8_t _dataBit3;
    uint8_t _dataBit2;
    uint8_t _dataBit1;
    uint8_t _dataBit0;
    uint8_t _busy;
    uint8_t _strobe;

    
    uint8_t _bitOrder;
    uint8_t previousCharacter;
    
    uint8_t fontSize = 1;
    uint8_t barcodeHeight = 50;
    bool paperCut;

    //Font settings
    bool bold = false;
    bool underline = false;
    bool inverted = false;
    bool strike = false;

    //Used to store print settings
    uint8_t printMode;
}; 

#endif

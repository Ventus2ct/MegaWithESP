#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>             // Arduino SPI library

// ST7789 TFT module connections
#define TFT_CS    LOW  // define chip select pin
#define TFT_DC     7  // define data/command pin
#define TFT_RST    8  // define reset pin, or set to -1 and connect to Arduino RESET pin

class DisplayST7789
{
public:
    DisplayST7789();    

private:
    void init(int x, int y, uint8_t spimode);
};
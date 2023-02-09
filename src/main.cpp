#include <Arduino.h>
#include <EspNetForMega.h>
#include <DisplayST7789.h>
#include <Adafruit_GFX.h>
#include <Arduino_ST7789_Fast.h>
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>

/*
 ST7789 240x240 IPS (without CS pin) connections (only 6 wires required):
 On UNO:
 #01 GND -> GND
 #02 VCC -> VCC (3.3V only!)
 #03 SCL -> D13/SCK
 #04 SDA -> D11/MOSI
 #05 RES -> D8 or any digital
 #06 DC  -> D7 or any digital
 #07 BLK -> NC
*/

// #define TFT_CS 10 // if your display has CS pin
// ON MEGA
#define TFT_DC 8 // data pin
#define TFT_RST 9 // reset pin (RES)
#define SCR_WD   240
#define SCR_HT   240   // 320 - to allow access to full 240x320 frame buffer

/* No luck using 20/21*/
// #define TFT_MOSI 51 // SDA
// #define TFT_SCLK 52 // SCL
// #define TFT_MOSI 21 // SDA
// #define TFT_SCLK 20 // SCL


Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

EspNetForMega espm;
// Adafruit_ST7789 lcd = Adafruit_ST7789(TFT_CS,TFT_DC,TFT_RST);
Arduino_ST7789 lcd = Arduino_ST7789(TFT_DC, TFT_RST);

float p = 3.1415926;

void testdrawtext(char *text, uint16_t color);

void setup() {
  Serial3.begin(115200);
  Serial.begin(115200);
  
  pinMode(13, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

   
  // lcd.init(240,240,SPI_MODE2); // Init ST7789 display 240x240 pixel
  lcd.init();
  // if the screen is flipped, remove this command
  lcd.setRotation(2);

  uint16_t time = millis();
  lcd.fillScreen(WHITE);
  Serial.println("Start");
  lcd.clearScreen();
  lcd.init(SCR_WD, SCR_HT);
  lcd.fillScreen(BLACK);
  lcd.setCursor(5, 0);
  lcd.setTextColor(WHITE,BLUE);
  lcd.setTextSize(2);
  lcd.println("Ea Buaroe");
  lcd.setCursor(5, 20);
  lcd.cp437(true);
  lcd.setTextSize(2);
  lcd.setTextColor(GREY);
  lcd.setCursor(10, 109);
  lcd.setFont(&FreeSans9pt7b);
  lcd.println("LRB Tech AS");
  //lcd.setBrightness()
  lcd.drawRoundRect( 0, 80, 240, 50, 5, DGREY); //x y width high radius color rectangle A 
  lcd.drawRoundRect( 2, 82, 238, 46, 5, WHITE); //x y width high radius color rectangle A 
  

  time = millis() - time;
  Serial.println(time, DEC);  
  delay(500);
 
  
  Serial.println("Staring");
  espm.SetSSID("aSid-Net","saab_9-5");
  //        NOTE: password 12345678 MUST have 8 characters 
  espm.setupAPStation();
  Serial.println("staring Web");
}

void loop() {
  espm.poll();
}
void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}
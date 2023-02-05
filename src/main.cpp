#include <Arduino.h>
#include <EspNetForMega.h>

EspNetForMega espm;

void setup() {
  Serial3.begin(115200);
  Serial.begin(115200);
  
  pinMode(13, OUTPUT);
  pinMode(3, OUTPUT);
  
  Serial.println("Staring");
  espm.SetSSID("aSid-Net","saab_9-5");
  //                       12345678 MUST have 8 characters 
  //espm.setupNet();
  espm.setupAPStation();
  Serial.println("staring Web");
}

void loop() {
  espm.poll();
}
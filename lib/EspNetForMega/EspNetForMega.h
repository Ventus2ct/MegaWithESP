#include "WiFiEspAT.h"

#ifndef ESPNETFORMEGA_H
#define ESPNETFORMEGA_H

#pragma once
/*  Wrap class for ESP8266 Embedded on MEGA or Mega With ESP-01 S
    Define as EspNetForMega espm; globally
    Insert espm.setupNet(); in main setup()
    Insert espm.poll(); in main loop()
    Add "lib_deps = jandrassy/WiFiEspAT@^1.4.1" via Platform IO library add
 */
class EspNetForMega
{
public:
    EspNetForMega();
    void setupNet();
    void poll();
    void pollold();
    void SetSSID(char *SSID, char *pass);
    long Baud = 115200;      // Baudrate
    void setupAPStation();

private:
    char *ssidName;           // your network SSID (name)
    char *passWord;           // your network password
    char *hostName;
    unsigned long delaytimer;
    void printCurrentNet();
    void printWifiData();
    String MainPage();
    char* readRequest(WiFiClient client);
    void execute(String action, String value);
    String getAction(char *request);
    String getStringBetween(char* input, char start, char end);
    String getValue(char *request);
    int WiFi_status;              // the Wifi radio's status
    void printApStatus();
    void printMacAddress(byte mac[]);
    bool SetupAP;
    void BlinkLed(bool On);

    IPAddress ip;
    IPAddress gw;
    IPAddress nm;
    IPAddress dns;
    
    bool myDebug;
    // Heater State true = on..
    bool HeaterState;
};

#endif
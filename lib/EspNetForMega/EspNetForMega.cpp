#include "EspNetForMega.h"
#include "WiFiEspAT.h"
#include "LowPower.h"
/*
WiFiEspAT:
- Use iPhone as Wifi AP
- Setup WebSever
- How to configure?
- AP som kun kan brukes som ap og webserver?
- IP adresse 192.168.4.1

#define WIFIESPAT_LOG_LEVEL LOG_LEVEL_DEBUG

*/


WiFiServer server(80);

EspNetForMega::EspNetForMega()
{
    myDebug = true;
    HeaterState = false;
    delaytimer = millis();

}
/* Call from Setup() */
void EspNetForMega::setupNet()
{

    Serial.println("Starting EspNetForMega");

    WiFi.init(&Serial3);
    Serial.print("Firmware version:");
    Serial.println(WiFi.firmwareVersion());

   // WiFi.init(Serial1);

    Serial.print("Connecting to: ");
    Serial.print(ssidName);
    Serial.println(passWord);
    Serial.println("Waiting for connection:");

    if (WiFi.status() == WL_NO_MODULE) {
        Serial.println();
        Serial.println("Communication with WiFi module failed!");
        // don't continue
        while (true);
    }
    Serial.println("Waiting for connection to WiFi");
    WiFi_status = WiFi.begin(ssidName, passWord );
    if (WiFi_status == WL_CONNECTED) {
        Serial.println();
        Serial.println("Connected to WiFi network. IP: ");
        Serial.println(WiFi.localIP());
        printWifiData();

    } else {
        WiFi.disconnect(); // remove the WiFi connection
        Serial.println();
        Serial.println("Connection to WiFi network failed.");
    }
}
void EspNetForMega::setupAPStation()
{
    int i = 0;

    Serial.println("Starting EspNetForMega");

    WiFi.init(&Serial3);
    Serial.print("Firmware version: ");
    Serial.println(WiFi.firmwareVersion());

    WiFi.disconnect(); // to clear the way. not persistent

    WiFi.setPersistent(); // set the following settings as persistent
    WiFi.endAP();

    //WiFi.configureAP(ip);
    ip = IPAddress(192,168,192,254);
    gw = IPAddress(192,168,192,10);
    nm = IPAddress(255,255,255,0);
    dns = IPAddress(192,168,192,40);
    WiFi.config(ip, dns, gw, nm);

    WiFi.setHostname("aSid");

    Serial.println("a Net With: ");
    Serial.print(ssidName);
    Serial.print(" ");
    Serial.println(passWord);

    Serial.println("Setting up AP");

    // while (WiFi.status() != WL_CONNECTED) {                         // attempt to connect to WiFi network
    //     // Serial.print("Attempting to connect to WPA SSID: ");    // Print message to serial monitor
    //     // Serial.println(ssidName);                                   // Print SSID to serial monitor
    //     Serial.print("\r. "+String(i));
    //     WiFi_status = WiFi.begin(ssidName, passWord);
    //     i +=1;                        // Connect to WPA/WPA2 network
    // }

    // WiFi.setPersistent(); // set the following settings as persistent
    // WiFi.endAP();

    //Serial.println("Connected, setting up AP");
    // set the following settings as persistent
    // WiFi_status = WiFi.beginEnterprise()
    // WiFi.encryptionType()
    WiFi_status = WiFi.beginAP(ssidName, passWord );

    if (WiFi_status == WL_AP_LISTENING) {
        Serial.println();
        delay(1000); // startup of AP
        Serial.println("AP started\n+r");
        printApStatus();
    }
    else
    {
        Serial.println();
        Serial.println("AP failed to start.");
    }
    // printCurrentNet();
    // printWifiData();
    delaytimer = millis();
    server.begin();
}
void EspNetForMega::printApStatus() {

  char ssid[33];
  WiFi.apSSID(ssid);
  Serial.println("Ap Status: ");
  Serial.println("-----------------");
  Serial.print("AP SSID: ");
  Serial.println(ssid);

  Serial.print("AP can handle ");
  Serial.print(WiFi.apMaxConnections());
  Serial.println(" stations.");

  if (WiFi.apEncryptionType() == ENC_TYPE_NONE) {
    Serial.println("AP is open (no encryption)");
  } else {
    Serial.print("Encryption of AP is ");
    switch (WiFi.apEncryptionType()) {
      case ENC_TYPE_TKIP:
        Serial.println("WPA.");
        break;
      case ENC_TYPE_CCMP:
        Serial.println("WPA2.");
        break;
    }
    char pass[65];
    WiFi.apPassphrase(pass);
    Serial.print("AP passphrase: ");
    Serial.println(pass);
    Serial.println("-----------------");
  }

  uint8_t mac[6];
  WiFi.apMacAddress(mac);
  Serial.print("AP MAC: ");
  printMacAddress(mac);

  IPAddress ip = WiFi.apIP();
  Serial.print("AP IP Address: ");
  Serial.println(ip);
}
void EspNetForMega::printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}
void EspNetForMega::BlinkLed(bool On)
{
    if(On)
    {
        digitalWrite(LED_BUILTIN,HIGH);
        Serial.println("Led ON");
    }   
    else
    {
        digitalWrite(LED_BUILTIN,LOW);
        Serial.println("Led off");
    }
}

/* Call from loop() */
void EspNetForMega::poll()
{
    // ToDo: Test sleep
    // See class myDebug=true;
    String line;
    WiFiClient client = server.available();
    String m = "";
    boolean currentLineIsBlank = true;

    // if(myDebug){
    //     Serial.print("\rIs A client connected: ");
    //     Serial.print(client.connected());
    //     Serial.print( ", ");
    // }

    if(client)
    {
        line = client.readStringUntil('\r');
        
        if(line.indexOf("heat=on") > -1) // strstr(line,"on"))
        {
           Serial.println("On detected"); 
           HeaterState=true;
           BlinkLed(true);
           // Turn relay on
           digitalWrite(3, LOW);
        }
      
        if(line.indexOf("heat=off") > -1)
        {
           Serial.println("Off detected"); 
           HeaterState=false;
           BlinkLed(false);
        // Turn relay off
           digitalWrite(3, HIGH);
        }
        //power=off
        if(line.indexOf("power=off") > -1)
        {
           Serial.println("Powering OFF");
           Serial.println();
           LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
        }
        // if(line.indexOf("favicon") > -1)
        // {
        // We execute the action with the value passed in the request
            //execute(getAction(line), getValue(request));
            //execute(getAction(WifiRequest), getvalue(WifiRequest));
            //Serial.println("Sending response to: " + String(request)) ;
            m=MainPage();
            // Serial.println(m);
            client.print(m);
            delay(100);
        // }
    }
    client.stop();
}

/* Call from loop() */
void EspNetForMega::pollold()
{

    // See class myDebug=true;
    WiFiClient client = server.available();
    String m = "";
    boolean currentLineIsBlank = true;

    // if(myDebug){
    //     Serial.print("\rIs A client connected: ");
    //     Serial.print(client.connected());
    //     Serial.print( ", ");
    // }

    if(client)
    {
        char* request = readRequest(client);
        //char request[500] = readRequest(client);
        //String WifiRequest = client.readStringUntil('\n');
        //if(WifiRequest.indexOf("favicon")== NULL)
        
        if(strstr(request,"on"))
        {
           Serial.println("On detected"); 
        }
      
        if(strstr(request,"off"))
        {
           Serial.println("Off detected"); 
        }
        if(strstr(request, "favicon") == NULL)
        {
        // We execute the action with the value passed in the request
            execute(getAction(request), getValue(request));
            //execute(getAction(WifiRequest), getvalue(WifiRequest));
            Serial.println("Sending response to: " + String(request)) ;
            m=MainPage();
            // Serial.println(m);
            client.print(m);
            delay(100);
        }
    }
    client.stop();
}

void EspNetForMega::execute(String action, String value)
{
    //if(myDebug) {
        Serial.println("Pin / Action; " + String(value) + "/" + String(action));
        //}
    //If it's one of the two actions we expect
    if(action == "on" || action == "off")
    {
        Serial.println("Inside 'action'" );
        //The relays are numbered starting from 1, max the array starts from 0 so we take 1
        int index = value.toInt() - 1;
        //The pin number will be the index plus the pin number where the relays start.
        //Relays must be in sequence from the start pin (FIRST_PIN)
        // int pinNumber = index;
        int status = action == "on" ? HIGH : LOW;
        if(action == "on")
            BlinkLed(true);
        else
            BlinkLed(false);
    }
}
//Returns the action the client wants to perform (on off)
String EspNetForMega::getAction(char *request)
{
    if(myDebug) {Serial.println("GetAction: " + String(request));}
    return getStringBetween(request, '?', '=');
}
//Returns the value (relay number) that the action will be executed
String EspNetForMega::getValue(char *request)
{
    return getStringBetween(request, '=', ' ');
}

String EspNetForMega::getStringBetween(char* input, char start, char end)
{
    String str = "";
    //returns the memory address of the 'start' character
    Serial.println("Get between " + String(start) + " and " + String(end) + " in string " + String(input));
    char* c = strchr(input, start);

    //If you didn't find the character
    if(c == NULL)
    {
        return "";
    }

    //Goes to the next character
    c++;

    //While not reaching the 'end' character or the end of the string
    while(*c != end && *c!='\0')
    {
        str += *c;
        c++;
    }

    return str;
}
String EspNetForMega::MainPage()
{
    //index_html=R"rawliteral(
    String ptr ;
    ptr += "HTTP/1.1 200 OK\r\n";
    ptr += "Content-Type: text/html\r\n";
    ptr += "Connection: close\r\n";
    // ptr += "Refresh: 5\r\n";
    ptr += "\r\n";

    ptr += "<!DOCTYPE html> <html>\n";
    ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no \r\nRefresh: 20\">\n";
    ptr +="</head><title>aSid Control</title>\n";
    ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
    ptr +="body{margin-top: 25px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
    ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
    ptr +="h1 {font-size: 1em;color: #888;margin-bottom: 10px;}\n";
    ptr +=".button {display: block;width: 100px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 16px;}\n";
    ptr +=".button_on {background-color: #3498db; colour: Green}\n";
    ptr +=".button_on:active {background-color: #2980b9;}\n";
    ptr +=".button_off {background-color: #34495e;}\n";
    ptr +=".button_off:active {background-color: #2c3e50;}</style>\n";
    ptr +="<body><html>\n";
    ptr +="<h1>aSID Web</h1>\n";
    ptr +="<p>Set heater: </p>\n";
    // ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
    if(HeaterState)
    {
        //On:
        ptr +="<button class=""button button_off"" onclick=\"location.href='?heat=off'\">OFF</button>\n";
       // HeaterState=false;
    }
    else
    {
        ptr +="<button class=""button button_on"" onclick=\"location.href='?heat=on'\">On</button>\n";
       // HeaterState=true;
    }
    ptr +="<p>aSID power: </p>\n";
    ptr +="<button class=""button button_on"" onclick=\"location.href='?power=off'\">Sleep aSID</button>\n";

    // ptr +="</br>millis; " + String(millis());
    ptr +="<p>LRB TECH as</p>\n";
    ptr +="</body></html>";
    return ptr;
}
//Reads the first line of the request
char* EspNetForMega::readRequest(WiFiClient client)
{
    bool currentLineIsBlank = true;
    char request[500];
    int i = 0;
    bool firstLine = true;

    while (client.connected()){
        if(client.available()){
            char c = client.read();
            // Serial.write(c);

            // Only the first line of the request interests us.
            if(firstLine){
                request[i] = c;
                i++;
            }

            if (c == '\n'){
                //The last line of the request is a \r\n by itself, after the \r\n of the previous line
                if(currentLineIsBlank)
                {
                    // readRequest;
                    //If you got here, it's because the request was read in full
                    break;
                }

                currentLineIsBlank = true;
                firstLine = false;
            }
            else if (c != '\r'){
                //If you read any character other than \n the \r means that the line is not blank
                currentLineIsBlank = false;
            }
            // request[i] = c;
            //     i++;
        }
    }
    Serial.println("--Request----------------");
    Serial.println(String(request));
    Serial.println("--END--------------------");
    request[i+1] = '\0';
    return request;
}
void EspNetForMega::SetSSID(char *SSID, char *pass)
{
    ssidName = SSID;
    passWord = pass;
}
// *********************************************************  FUNCTION DEFINITIONS *********************************************************
// Print WiFi data to serial Monitor  --------------------------------------------------------
void EspNetForMega::printWifiData()
{
  // *** print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // *** print your MAC address
  byte mac[6];
  WiFi.macAddress(mac);
  char buf[20];
  sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
  Serial.print("MAC address: ");
  Serial.println(buf);
}
// Print WiFi connection details to serial Monitor  --------------------------------------------------------
void EspNetForMega::printCurrentNet()
{
  // *** print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // *** print the MAC address of the router you're attached to
  byte bssid[6];
  WiFi.BSSID(bssid);
  char buf[20];
  sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", bssid[5], bssid[4], bssid[3], bssid[2], bssid[1], bssid[0]);
  Serial.print("BSSID: ");
  Serial.println(buf);

  // *** print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.println(rssi);
}
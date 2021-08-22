/*
 * ESP32 AJAX Demo
 * Updates and Gets data from webpage without page refresh
 * https://circuits4you.com
 */
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <Wire.h> 
#include <Adafruit_GFX.h> 
#include "Adafruit_LEDBackpack.h" 
#define SEND_DELAY 200

#include "index.h"  //Web page header file
    
int outputValue = 0;  
Adafruit_8x16minimatrix matrix = Adafruit_8x16minimatrix(); 
WebServer server(80);

//Enter your SSID and PASSWORD
const char* ssid = "";
const char* password = "";
const int analogInPin = 36; 
//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
 
void handleADC() {
 int a = analogRead(analogInPin);
 String adcValue = String(a);
 
  outputValue = map(a, 0, 1023, 0, 99);  
 
  Serial.print("sensor = ");     Serial.print(a);  
  Serial.print("\t output = ");  Serial.println(outputValue); 

  //Display value on LED Matrix
  matrix.clear();
  matrix.setCursor(3,0);
  String response = String(outputValue);
  Serial.println(response);
  matrix.print(response); 
  matrix.writeDisplay();

  delay(SEND_DELAY); 
 
 server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
}

//===============================================================
// Setup
//===============================================================

void setup(void){
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");
  analogReadResolution(10);
  matrix.begin(0x70);
  matrix.setRotation(1); 
  matrix.setTextSize(1); 
  matrix.setTextWrap(false); 
  matrix.setTextColor(LED_ON);
/*
//ESP32 As access point
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);
*/
//ESP32 connects to your wifi -----------------------------------
  WiFi.mode(WIFI_STA); //Connectto your wifi
  WiFi.begin(ssid, password);

  Serial.println("Connecting to ");
  Serial.print(ssid);

  //Wait for WiFi to connect
  while(WiFi.waitForConnectResult() != WL_CONNECTED){      
      Serial.print(".");
    }
    
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
//----------------------------------------------------------------
 
  server.on("/", handleRoot);      //This is display page
  server.on("/readADC", handleADC);//To get update of ADC Value only
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void loop(void){
  
  server.handleClient();
  delay(1);
}

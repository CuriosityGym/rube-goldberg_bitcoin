/*
 *  HTTP over TLS (HTTPS) example sketch
 *
 *  This example demonstrates how to use
 *  WiFiClientSecure class to access HTTPS API.
 *  We fetch and display the status of
 *  esp8266/Arduino project continuous integration
 *  build.
 *
 *  Created by Ivan Grokhotkov, 2015.
 *  This example is in public domain.
 */

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "New Curiosity Gym";
const char* password = "CuriosityTwinkle";

const char* host = "bitcoinsample.herokuapp.com";
const int httpsPort = 443;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char* fingerprint = "CF 05 98 89 CA FF 8E D8 5E 5C E0 C2 E4 F7 E6 C3 C7 50 DD 5C";
const char* DEVICE_ID  = "a2";
const int BUTTON_PIN = 0;
//////////////////////
// Button Variables //
//////////////////////
int BUTTON_STATE;
int LAST_BUTTON_STATE = HIGH;
long LAST_DEBOUNCE_TIME = 0;
long DEBOUNCE_DELAY = 100;
int BUTTON_COUNTER = 0;
int IDLE_DELAY=5000;
int lastPressedMillis=0;
boolean buttonPressed = false;

// Debounce Button Presses
boolean debounce() {
  boolean retVal = false;
  int reading = digitalRead(BUTTON_PIN);
  if (reading != LAST_BUTTON_STATE) {
    LAST_DEBOUNCE_TIME = millis();
  }
  if ((millis() - LAST_DEBOUNCE_TIME) > DEBOUNCE_DELAY) {
    if (reading != BUTTON_STATE) {
      BUTTON_STATE = reading;
      if (BUTTON_STATE == LOW) {
        retVal = true;
      }
    }
  }
  LAST_BUTTON_STATE = reading;
  return retVal;
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


}

void loop() {
  // Wait for button Presses
  boolean pressed = debounce();
  if (pressed == true && buttonPressed == false)
  {
      //if(BUTTON_COUNTER==EVENTS)// This will make the Button Counter roll back to 1
      //{
      //  BUTTON_COUNTER=0;
      //}
      //BUTTON_COUNTER++;
      //Serial.print(BUTTON_COUNTER);
     
      lastPressedMillis=millis();  

  }

    if((millis()-lastPressedMillis)>IDLE_DELAY && lastPressedMillis>0)  //Wait for " IDLE_DELAY" Time to ensure person has clicked switch the right number of times
      { 
         lastPressedMillis=0;
           // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
  }

  String url = "/device/" + String(DEVICE_ID);;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  /*
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");*/
  //BUTTON_COUNTER=0;
  delay(5000);
      }
      
}

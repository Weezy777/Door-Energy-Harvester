#include <Arduino.h>
#include <Wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h" 



float getCurrent(){

  return random(2.5,10);
}

float getVoltage(){
 return random(2,5);
}

int getEff(){
  return random(0,100);
}

#define endpoint "https://door-energy-harvester-api.onrender.com/data"





void setup() {
 Serial.begin(9600);

	// WiFi_SSID and WIFI_PASS should be stored in the env.h
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("");
	// Connect to wifi
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){

    HTTPClient http;
  
    // Establish a connection to the server
    http.begin(endpoint);
    

       // Specify content-type header
    http.addHeader("Content-Type", "application/json");

    // Serialise JSON object into a string to be sent to the API
    StaticJsonDocument<64> doc;
    String httpRequestData;

    float current = getCurrent();
    float voltage = getVoltage();
    float efficiency = getEff();



    doc["current"] = current;
    doc["voltage"] = voltage;
    doc["efficiency"] = efficiency;


    serializeJson(doc, httpRequestData);

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
    String http_response;

    // check reuslt of POST request. negative response code means server wasn't reached
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      Serial.print("HTTP Response from server: ");
      http_response = http.getString();
      Serial.println(http_response);

    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
     delay(2000);
   
   
    // Free resources
    http.end();
   
  }
  else {
    Serial.println("WiFi Disconnected");
  }

}

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

#define EXPRESS_SERVER "http://192.168.22.146:4000"

ESP8266WiFiMulti WiFiMulti;
bool readyToRequest;
const String healthCheckUrl = String(EXPRESS_SERVER) + "/healthcheck";
const String carEnteringUrl = String(EXPRESS_SERVER) + "/api/v1/cards/linked-vehicle?cardId=";
const String updateParkingSlotUrl = String(EXPRESS_SERVER) + "/api/v1/parking-slots";
WiFiClient client;
HTTPClient http;

void setup() {
  Serial.begin(9600);
  readyToRequest = false;

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Trung Tam TT-TV", "12345679");
}

String urlencode(const String &str) {
    String encoded = "";
    for (int i = 0; i < str.length(); i++) {
        char c = str[i];
        if (isalnum(c)) {
            encoded += c;
        } else {
            encoded += '%';
            char buf[3];
            sprintf(buf, "%02X", c);
            encoded += buf;
        }
    }
    return encoded;
}

void requestForCarEntering (String value) {
  http.setTimeout(30000);
  String url = carEnteringUrl + urlencode(value);

  if (!http.begin(client, url)) {
    readyToRequest = false;
    Serial.println("[HTTP] Unable to connect");
    return;
  }

  Serial.println("[HTTP] GET: " + url);
  int httpCode = http.GET();

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      String payload = http.getString();
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.println("JSON parse failed");
        return;
      }

      String info = doc["info"].as<String>();

      Serial.println("USER:" + info);
      Serial.println("CHECKING-RESULT:1");
    } else {
      Serial.println("CHECKING-RESULT:0");
    }
  } else {
    readyToRequest = false;
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void requestToUpdateParkingState (String value) {
  http.setTimeout(30000);
  if (http.begin(client, updateParkingSlotUrl)) {
    http.addHeader("Content-Type", "application/json");
    Serial.println("[HTTP] PUT: " + updateParkingSlotUrl);
    String payload = "{\"states\":\"" + value +  "\"}";
    int httpCode = http.PUT(payload);
    
    if (httpCode > 0) {
      Serial.printf("[HTTP] PUT... code: %d\n", httpCode);
    } else {
      readyToRequest = false;
      Serial.printf("[HTTP] PUT... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    readyToRequest = false;
    Serial.println("[HTTP] Unable to connect");
  }
}

void pingToExpressServer () {
  http.setTimeout(2000);
  if (http.begin(client, healthCheckUrl)) {
    Serial.println("[HTTP] GET: " + healthCheckUrl);
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.printf("[HTTP] GET: healthcheck code: %d\n", httpCode);
      readyToRequest = true;

      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] GET healthcheck failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    Serial.println("[HTTP] Unable to connect");
  }
}

void loop() {
  if ((WiFiMulti.run() == WL_CONNECTED)) { // wifi is ready
    if(readyToRequest){ // express server is ready
      if((Serial.available() > 0)){ //Serial is ready
        String input = Serial.readStringUntil('\n');
        int separatorIndex = input.indexOf(':');

        if (separatorIndex != -1) {
          String label = input.substring(0, separatorIndex);
          String value = input.substring(separatorIndex + 1);

          if(label == "CARD"){
            requestForCarEntering(value);
          } else if(label == "STATE") {
            value.trim();
            requestToUpdateParkingState(value);
          }
        }  
      }
    } else {
      pingToExpressServer();
      delay(1000);
    }
  }
}
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define WRITE_TO_MEGA_PIN D5
#define EXPRESS_SERVER "http://192.168.22.146:4000"

ESP8266WiFiMulti WiFiMulti;
bool isQuerying;
bool readyToRequest;
String healthCheckUrl;
String carEnteringUrl;
WiFiClient client;
HTTPClient http;

void setup() {
  Serial.begin(9600);
  pinMode(WRITE_TO_MEGA_PIN, OUTPUT);
  digitalWrite(WRITE_TO_MEGA_PIN, LOW);
  isQuerying = false;
  readyToRequest = false;
  healthCheckUrl = String(EXPRESS_SERVER) + "/healthcheck";
  carEnteringUrl = String(EXPRESS_SERVER) + "/api/v1/cards/linked-vehicle?cardId=";

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
  String url = carEnteringUrl + urlencode(value);
  if (http.begin(client, url) && !isQuerying) {
    isQuerying = true;
    Serial.println("[HTTP] GET: " + url);
    int httpCode = http.GET();
    Serial.println(url);

    if (httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      digitalWrite(WRITE_TO_MEGA_PIN, HIGH);

      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      readyToRequest = false;
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
    isQuerying = false;
  } else {
    readyToRequest = false;
    Serial.println("[HTTP] Unable to connect");
  }
}

void pingToExpressServer () {
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
          }
        }  
      }
    } else {
      pingToExpressServer();
      delay(1000);
    }
  }

  delay(1000);
  digitalWrite(WRITE_TO_MEGA_PIN, LOW);
}



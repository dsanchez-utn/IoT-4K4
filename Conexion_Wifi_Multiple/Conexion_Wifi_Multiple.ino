#include <WiFi.h>
#include <WiFiMulti.h>
#include "redes.h"

WiFiMulti wifiMulti;

void setup() {
  Serial.begin(115200);
  delay(10);

  wifiMulti.addAP(SSID1, PASS1);
  wifiMulti.addAP(SSID2, PASS2);
  wifiMulti.addAP(SSID3, PASS3);


  Serial.println("Connecting Wifi...");
  if (wifiMulti.run() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void loop() {
  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("WiFi not connected!");
    delay(1000);
  }
}

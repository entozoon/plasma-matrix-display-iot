#include <Arduino.h>
#include <MS6205.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <Conf.h>
int const shiftRegisterLatchPin = D8;
int const shiftRegisterClockPin = D5;
int const shiftRegisterDataPin = D7;
int const displaySetPositionPin = D6;
int const displaySetCharacterPin = D4;
int const displayClearPin = D1;
MS6205 display(shiftRegisterLatchPin, shiftRegisterClockPin, shiftRegisterDataPin, displaySetPositionPin, displaySetCharacterPin, displayClearPin);
WiFiClient api;
void wifiConnect()
{
  if (WiFi.status() == WL_CONNECTED)
    return;
  display.clear();
  display.setCursor(0, 0);
  display.write("MichaelCook.tech");
  display.setCursor(0, 3);
  display.write("Connecting WIFI");
  Serial.println("Connecting WIFI");
  WiFi.mode(WIFI_STA);
  WiFi.hostname("plasma-matrix-display-iot");
  WiFi.begin(wifiSsid, wifiPass);
  while (WiFi.status() != WL_CONNECTED)
  {
    display.write(".");
    delay(500);
  }
}
String apiQuery()
{
  std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
  // Fingerprint expires on June 2, 2021, needs to be updated well before this date
  // const uint8_t fingerprint[20] = {0x40, 0xaf, 0x00, 0x6b, 0xec, 0x90, 0x22, 0x41, 0x8e, 0xa3, 0xad, 0xfa, 0x1a, 0xe8, 0x25, 0x41, 0x1d, 0x1a, 0x54, 0xb3};
  // client->setFingerprint(fingerprint);
  // Update: no need for fingerprint! Bit insecure, apparently but meh: https://buger.dread.cz/simple-esp8266-https-client-without-verification-of-certificate-fingerprint.html
  client->setInsecure();
  HTTPClient https;
  String payload;
  Serial.print("[HTTPS] begin...\n");
  if (https.begin(*client, query))
  {
    Serial.print("[HTTPS] GET...\n");
    int httpCode = https.GET();
    if (httpCode > 0)
    {
      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        payload = https.getString();
        // Serial.println(payload);
      }
    }
    else
    {
      Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
    }
    https.end();
    if (payload)
    {
      // Serial.println(payload);
      return payload;
    }
  }
}
void setup()
{
  Serial.begin(115200);
  display.clear();
  delay(4000);
  wifiConnect(); // Initial, for sake of IP display
  display.setCursor(0, 6);
  display.write("Connected as ");
  Serial.print("Connected as ");
  display.setCursor(0, 7);
  display.write(WiFi.localIP().toString().c_str());
  Serial.println(WiFi.localIP().toString().c_str());
  delay(2000);
}
void loop()
{
  wifiConnect(); // as necessary
  String result = apiQuery();
  if (result)
  {
    display.clear();
    char resultChars[result.length() + 1];
    result.toCharArray(resultChars, result.length());
    const char *delim = "\r\n";
    // Returns first token
    char *line = strtok(resultChars, delim);
    // Keep printing tokens til no more delims
    int i = 0;
    int limit = 10;
    while (line != NULL && i < limit)
    {
      line = strtok(NULL, delim);
      display.setCursor(0, i);
      display.write(line);
      Serial.println(line);
      i++;
    }
    Serial.println("");
    delay(60 * 60 * 1000);
  }
  delay(5000);
}
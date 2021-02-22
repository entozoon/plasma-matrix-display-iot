#include <Arduino.h>
#include <MS6205.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Conf.h>
int const shiftRegisterLatchPin = D8;
int const shiftRegisterClockPin = D5;
int const shiftRegisterDataPin = D7;
int const displaySetPositionPin = D6;
int const displaySetCharacterPin = D4;
int const displayClearPin = D1;
MS6205 display(shiftRegisterLatchPin, shiftRegisterClockPin, shiftRegisterDataPin, displaySetPositionPin, displaySetCharacterPin, displayClearPin);
WiFiClient client;
void wifiConnect()
{
  display.clear();
  display.write("Connecting WIFI ");
  WiFi.mode(WIFI_STA);
  WiFi.hostname("plasma-investments");
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    display.write(".");
    delay(100);
  }
}
boolean wifiQuery()
{
  Serial.print("Requesting URL: ");
  Serial.print(HOST);
  Serial.println(ENDPOINT);
  client.print(String("GET ") + ENDPOINT +
               " HTTP/1.1\r\n" +
               "Host: " + HOST + "\r\n" +
               "Connection: close\r\n\r\n");
  // Wait a while for response
  int c = 0;
  while ((!client.available()) && (c < 400)) // 20 = 12s at 50ms delay, 400 = 40s at 100ms delay (it can take fucking ages for puppeteer)
  {
    // ringFill(0, 0, 100, 25); // Blue, subtle
    delay(100);
    c++;
  }
  String result = "";
  while (client.available())
  {
    char ch = static_cast<char>(client.read());
    // Serial.print(ch);
    result += ch;
  }
  client.stop();
  // trim out the server nonsense that gets appended to the result
  String data = result.substring(result.lastIndexOf('\n'));
  Serial.println(data);
  // Abort, for a minute then yeah
  if (data.indexOf("\"success\":true") == -1 || data.indexOf("\"success\":false") != -1 || data.length() < 5)
  {
    Serial.println("Aborting");
    ringFill(255, 0, 0, 25); // Red
    delay(60000);
    return false;
  }
  // In stock baby!
  if (data.indexOf("\"inStock\":true") != -1)
  {
    return true;
  }
  return false;
}
void setup()
{
  Serial.begin(115200);
  delay(4000);
  wifiConnect(); // Initial, for sake of IP display
  display.setCursor(0, 0);
  display.write("Connected as ");
  display.setCursor(0, 1);
  display.write(WiFi.localIP().toString().c_str());
  delay(2000);
}
void loop()
{
  wifiConnect(); // as necessary
  wifiClient();
  if (wifiQuery())
  {
  }
  // display.clear();
  // //A
  // // B
  // //  C
  // //   D
  // //    E
  // //     F
  // //      G
  // //       H
  // //        I
  // //         J
  // display.setCursor(0, 0);
  // display.write("A");
  // delay(1000);
  // display.setCursor(1, 1);
  // display.write("B");
  // delay(1000);
  // display.setCursor(2, 2);
  // display.write("C");
  // delay(1000);
  // display.setCursor(3, 3);
  // display.write("D");
  // delay(1000);
  // display.setCursor(4, 4);
  // display.write("E");
  // delay(1000);
  // display.setCursor(5, 5);
  // display.write("F");
  // delay(1000);
  // display.setCursor(6, 6);
  // display.write("G");
  // delay(1000);
  // display.setCursor(7, 7);
  // display.write("H");
  // delay(1000);
  // display.setCursor(8, 8);
  // display.write("I");
  // delay(1000);
  // display.setCursor(9, 9);
  // display.write("J");
  // delay(1000);
}
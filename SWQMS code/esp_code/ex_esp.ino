#define BLYNK_TEMPLATE_ID "TMPL3TsHyOJ3k"
#define BLYNK_TEMPLATE_NAME "SWQMS"
#define BLYNK_AUTH_TOKEN "XpBT_btUl_46R-1RP9RtaZP-lIuo_fVu"

#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

// WiFi credentials
char ssid[] = "Vivo";
char pass[] = "12345678";

BlynkTimer timer;

float tdsValue = 0.0;
float turbidityPercentage = 0.0;

void receiveDataFromArduino() {
  while (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    data.trim();

    int commaIndex = data.indexOf(',');

    if (commaIndex != -1) {
      tdsValue = data.substring(0, commaIndex).toFloat();
      turbidityPercentage = data.substring(commaIndex + 1).toFloat();

      Serial.print("TDS: ");
      Serial.print(tdsValue, 2);
      Serial.print(" ppm, Turbidity: ");
      Serial.print(turbidityPercentage, 1);
      Serial.println("%");

      // Send to Blynk
      Blynk.virtualWrite(V1, tdsValue);
      Blynk.virtualWrite(V2, turbidityPercentage);
    }
  }
}

void setup() {
  Serial.begin(9600); // Communication with Arduino Uno
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, receiveDataFromArduino); // Call every second
}

void loop() {
  Blynk.run();
  timer.run();
}

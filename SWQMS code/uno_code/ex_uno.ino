#include <GravityTDS.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// TDS Sensor Settings
#define TdsSensorPin A2
GravityTDS gravityTds;
float tdsValue = 0.0; // TDS value

// Turbidity Sensor Settings
#define TurbiditySensorPin A3
const float ADC_REF_VOLTAGE = 5.0;
const int ADC_RESOLUTION = 1024;

// Turbidity percentage range
const float TURBIDITY_MIN_VOLTAGE = 0.0; // Voltage corresponding to 0% turbidity
const float TURBIDITY_MAX_VOLTAGE = 5.0; // Voltage corresponding to 100% turbidity

float turbidityPercentage = 0.0;

void setup() {
  Serial.begin(9600); // Communication with ESP8266
  Serial.println("Arduino Uno Ready");

  // OLED initialization
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Initializing...");
  display.display();
  delay(2000);

  // Initialize TDS sensor
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(ADC_REF_VOLTAGE);
  gravityTds.setAdcRange(ADC_RESOLUTION);
  gravityTds.begin();
}

void loop() {
  // Read TDS
  gravityTds.update();
  tdsValue = gravityTds.getTdsValue();

  // Read turbidity and calculate percentage
  int turbiditySensorValue = analogRead(TurbiditySensorPin);
  float turbidityVoltage = turbiditySensorValue * (ADC_REF_VOLTAGE / ADC_RESOLUTION);
  turbidityPercentage = map(turbidityVoltage * 100, TURBIDITY_MIN_VOLTAGE * 100, TURBIDITY_MAX_VOLTAGE * 100, 0, 100);

  // Display on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("TDS: ");
  display.print(tdsValue, 2);
  display.println(" ppm");

  display.setCursor(0, 20);
  display.print("Turbidity: ");
  display.print(turbidityPercentage, 1);
  display.println("%");

  display.display();

  // Send data to NodeMCU (formatted string)
  Serial.print(tdsValue, 2);
  Serial.print(",");
  Serial.println(turbidityPercentage, 1);

  delay(1000);
}

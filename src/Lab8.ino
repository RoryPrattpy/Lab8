#define LED_CLOSE A0
#define LED_MEDIUM A1
#define LED_FAR A2
#define TEXT_DATA V9
#define NUM_DATA V8

SYSTEM_THREAD(ENABLED);

#include "env.h"
#include <Wire.h>
#include "oled-wing-adafruit.h"
#include "SparkFun_VCNL4040_Arduino_Library.h"
#include <blynk.h>

BlynkTimer timer;

OledWingAdafruit display;

VCNL4040 proximitySensor;

bool sendToApp = false;

void setup() {
  pinMode(LED_CLOSE, OUTPUT);
  pinMode(LED_MEDIUM, OUTPUT);
  pinMode(LED_FAR, OUTPUT);

  display.setup();
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN);

  display.clearDisplay();
	display.display();

  //while (!Serial.isConnected()) {}
  delay(1000);
  Serial.println("SparkFun VCNL4040 Example");

  Wire.begin();
  if (!proximitySensor.begin()) {
    Serial.println("Device not found. Please check wiring.");
  }
}

void loop() {
  Blynk.run();
  timer.run();
  display.loop();

  unsigned int proxValue = proximitySensor.getProximity(); 

  if (proxValue > 1000) {
    set_all_pins_off();
    digitalWrite(LED_CLOSE, 1);
    if (display.pressedB() && !sendToApp) {
      displayWrite(0, "Close Distance", false);
    } else if (sendToApp && display.pressedB()) {
      Blynk.virtualWrite(TEXT_DATA, "Close Distance");
    }
  } else if (proxValue > 150) {
    set_all_pins_off();
    digitalWrite(LED_MEDIUM, 1);
    if (display.pressedB() && !sendToApp) {
      displayWrite(0, "Medium Distance", false);
    } else if (sendToApp && display.pressedB()) {
      Blynk.virtualWrite(TEXT_DATA, "Medium Distance");
    }
  } else {
    set_all_pins_off();
    digitalWrite(LED_FAR, 1);
    if (display.pressedB() && !sendToApp) {
      displayWrite(0, "Far Distance", false);
    } else if (sendToApp && display.pressedB()) {
      Blynk.virtualWrite(TEXT_DATA, "Far Distance");
    }
  }

  if (display.pressedA() && !sendToApp) {
    displayWrite(proxValue, "", true);
  } else if (sendToApp && display.pressedA()) {
    Blynk.virtualWrite(NUM_DATA, proxValue);
  }

  if (display.pressedC()) {
    display.clearDisplay();
    display.display();
    sendToApp = !sendToApp;
  }
}

void set_all_pins_off() {
  digitalWrite(LED_CLOSE, 0);
  digitalWrite(LED_MEDIUM, 0);
  digitalWrite(LED_FAR, 0);
}


void displayWrite(int number, char *stringy, bool writeNum) {
  display.clearDisplay();
  display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,0);
  if (writeNum) {
    display.println(number);
  } else {
    display.println(stringy);
  }
	display.display();
}
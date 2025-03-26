#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

// HX711 Load Cell Pins
#define DT_PIN 2  // HX711 Data Pin
#define SCK_PIN 3  // HX711 Clock Pin
// L298N Motor Driver Pins
#define IN1 5   // Motor A Direction
#define IN2 6   // Motor A Direction
#define ENA 9   // Motor A Speed (PWM)

//pins:
const int HX711_dout = DT_PIN; //mcu > HX711 dout pin
const int HX711_sck = SCK_PIN; //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;

void setup() {
  Serial.begin(57600); delay(10);
  Serial.println();
  Serial.println("Starting...");

  LoadCell.begin();
  //LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive
  float calibrationValue; // calibration value (see example file "Calibration.ino")
  calibrationValue = 696.0; // uncomment this if you want to set the calibration value in the sketch
#if defined(ESP8266)|| defined(ESP32)
  //EEPROM.begin(512); // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
#endif
  //EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the calibration value from eeprom

  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }

    // Setup L298N motor control pins
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENA, OUTPUT);

    // Set motor direction forward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
}

void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;
  float weight;
  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      weight = LoadCell.getData();
      Serial.print("Load_cell output val: ");
      Serial.println(weight);
      newDataReady = 0;
      t = millis();
    }
  }
      // Map weight (0-500g) to motor speed (100-255 PWM)
    int motorSpeed = map(weight, 0, 500, 100, 255);

    // Ensure motor runs only when weight is detected
    if (weight > 10) {  // Adjust threshold as needed
        analogWrite(ENA, motorSpeed);  // Adjust motor speed
        Serial.print("Motor Speed: ");
        Serial.println(motorSpeed);
        Serial.print(weight);
    } else {
        analogWrite(ENA, 0);  // Stop motor when no weight
        Serial.println("Motor Stopped");
    }
    //delay(500);  // Small delay to avoid rapid changes
}


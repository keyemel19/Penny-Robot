#include <HX711.h>
// HX711 Load Cell Pins
#define DT_PIN 2  // HX711 Data Pin
#define SCK_PIN 3  // HX711 Clock Pin
// L298N Motor Driver Pins
#define IN1 5   // Motor A Direction
#define IN2 6   // Motor A Direction
#define ENA 9   // Motor A Speed (PWM)
HX711 scale;  // Create HX711 object
void setup() {
    Serial.begin(9600);  // Start Serial Monitor

    // Initialize HX711
    scale.begin(DT_PIN, SCK_PIN);
    scale.set_scale();  // Adjust this for calibration
    scale.tare();  // Reset scale to zero

    // Setup L298N motor control pins
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENA, OUTPUT);

    // Set motor direction forward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
}
void loop() {
    float weight = scale.get_units(10);  // Read weight (average over 10 readings)
    Serial.print("Weight: ");
    Serial.println(weight);  // Print weight to Serial Monitor

    // Map weight (0-500g) to motor speed (100-255 PWM)
    int motorSpeed = map(weight, 0, 500, 100, 255);

    // Ensure motor runs only when weight is detected
    if (weight > 10) {  // Adjust threshold as needed
        analogWrite(ENA, motorSpeed);  // Adjust motor speed
        Serial.print("Motor Speed: ");
        Serial.println(motorSpeed);
    } else {
        analogWrite(ENA, 0);  // Stop motor when no weight
        Serial.println("Motor Stopped");
    }
    //delay(500);  // Small delay to avoid rapid changes
}

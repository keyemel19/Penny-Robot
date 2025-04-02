const int vibrationPin = 2;    // Connect to the sensor's DO pin
const int in1 = 8;             // L298N IN1
const int in2 = 9;             // L298N IN2
const int enablePin = 10;      // L298N ENA (PWM for speed)

void setup() {
  pinMode(vibrationPin, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  
  // Start with motor off
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enablePin, 0);

  Serial.begin(9600);
}

void loop() {
  int vibrationDetected = digitalRead(vibrationPin);
  
  if (vibrationDetected == HIGH) {
    Serial.println("Vibration detected! Fan ON.");
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enablePin, 6000);  // You can adjust the speed (0–255)
    //delay(90);  // Let it spin up
    //analogWrite(enablePin, 250);  // Then slow down 
  } else {
    Serial.println("No vibration. Fan OFF.");
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(enablePin, 0);
  }

  delay(100);  // Small debounce delay
}

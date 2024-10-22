const int PulseSensorPin = 34; // GPIO34 for the analog input

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for the serial port to connect
  }
  Serial.println("Pulse sensor test started");
}

void loop() {
  int pulseValue = analogRead(PulseSensorPin); // Read the raw analog value
  Serial.print("Pulse sensor raw value: ");
  Serial.println(pulseValue); // Print the raw value for debugging

  delay(100); // Short delay to avoid flooding the serial monitor
}

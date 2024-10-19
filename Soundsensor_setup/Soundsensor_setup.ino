// Pin definitions
const int soundSensorPin = 34; // Analog pin for sound sensor (ESP32 ADC pin)

void setup() {
  // Start Serial communication
  Serial.begin(115200);
  Serial.println("Reading Analog Sound Sensor Values...");
}

void loop() {
  // Read the analog value from the sound sensor
  int soundValue = analogRead(soundSensorPin);

  // Print the analog value to the Serial Monitor
  Serial.print("Sound Sensor Value: ");
  Serial.println(soundValue);

  // Delay to make it readable in the Serial Monitor
  delay(1000);
}

// Pin definitions
const int soundSensorPin = 34; // Analog pin for sound sensor
const int redLEDPin = 26;      // Pin for red LED
const int greenLEDPin = 25;    // Pin for green LED

// Define your threshold value (set this based on your testing from Part 1)
int noiseThreshold = 11;

void setup() {
  // Initialize pins
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);

  // Start Serial communication for debugging
  Serial.begin(115200);
  Serial.println("Noise Control System Initialized with Analog Sound Sensor");
}

void loop() {
  // Read the analog value from the sound sensor
  int soundValue = analogRead(soundSensorPin);

  // Print the analog value and threshold to the Serial Monitor
  Serial.print("Sound Sensor Value: ");
  Serial.println(soundValue);

  // Check if the sound value exceeds the noise threshold
  if (soundValue > noiseThreshold) {
    Serial.println("Noise Detected: Exceeding Threshold");

    // Noise threshold exceeded, blink red LED
    digitalWrite(redLEDPin, HIGH);  // Turn on red LED
    delay(500);                     // Keep it on for 500ms
    digitalWrite(redLEDPin, LOW);   // Turn off red LED
    delay(500);                     // Wait 500ms

    // Display noise status in the serial monitor
    Serial.println("There is noise");

  } else {
    Serial.println("Noise Level Below Threshold");

    // Noise level settled, turn on green LED for a few seconds
    digitalWrite(greenLEDPin, HIGH); // Turn on green LED
    delay(2000);                     // Keep it on for 2 seconds
    digitalWrite(greenLEDPin, LOW);  // Turn off green LED
    delay(500);                      // Wait 500ms before next check

    // Display no-noise status in the serial monitor
    Serial.println("There is no noise");
  }

  // Delay for readability
  delay(100);
}

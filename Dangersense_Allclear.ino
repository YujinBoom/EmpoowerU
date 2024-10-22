// Pin definitions
const int soundSensorPin = 34;  // Analog pin for sound sensor
const int redLEDPin = 13;       // Pin for red LED
const int greenLEDPin = 12;     // Pin for green LED

// Define your threshold value for detecting danger noise
int noiseThreshold = 700;
unsigned long alertDuration = 5000;  // 5 seconds for the LEDs to stay on after a spike
unsigned long lastSpikeTime = 0;     // To track when the last spike occurred
bool alertActive = false;            // To track if an alert is currently active

void setup() {
  // Initialize pins
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);

  // Start Serial communication for debugging
  Serial.begin(115200);
  Serial.println("Danger Sense Detector Initialized");
}

void loop() {
  // Read the analog value from the sound sensor
  int soundValue = analogRead(soundSensorPin);

  // Print the sound sensor value for debugging
  Serial.print("Sound Sensor Value: ");
  Serial.println(soundValue);

  // Check if the sound value exceeds the noise threshold
  if (soundValue > noiseThreshold) {
    Serial.println("Dangerous Noise Spike Detected!");

    // Record the time of the noise spike
    lastSpikeTime = millis();
    
    // Activate the alert
    alertActive = true;
  }

  // Check if the alert is active and how long it should stay active
  if (alertActive) {
    // Turn on the red LED for danger alert
    digitalWrite(redLEDPin, HIGH); 
    
    // Keep the green LED off during an active alert
    digitalWrite(greenLEDPin, LOW);

    // If the alert has lasted longer than the defined duration, turn off the alert
    if (millis() - lastSpikeTime >= alertDuration) {
      alertActive = false;  // Deactivate the alert after 5 seconds
    }
  } else {
    // If no noise spikes and the alert period is over, turn off the red LED
    digitalWrite(redLEDPin, LOW);
    
    // Turn on the green LED to indicate safe/normal status
    digitalWrite(greenLEDPin, HIGH);
  }

  // Small delay for readability
  delay(100);
}

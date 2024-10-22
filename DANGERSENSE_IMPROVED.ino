// Pin definitions
const int soundSensorPin = 34;  // Analog pin for sound sensor
const int redLEDPin = 13;       // Pin for red LED
const int greenLEDPin = 12;     // Pin for green LED

// Define threshold for noise detection
int noiseThreshold = 700;
unsigned long alertDuration = 5000;      // How long to keep LEDs on after noise (milliseconds)
unsigned long greenLEDDuration = 3000;   // How long to keep green LED on after no noise

// Timing variables
unsigned long redLEDStartTime = 0;
unsigned long greenLEDStartTime = 0;
bool alertActive = false;
bool safeDeclared = false;

void setup() {
  // Initialize pins
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);

  // Start Serial communication for debugging
  Serial.begin(115200);
  Serial.println("Danger Sense Detector Initialized");
}

void loop() {
  // Read the sound sensor value
  int soundValue = analogRead(soundSensorPin);
  Serial.print("Sound Sensor Value: ");
  Serial.println(soundValue);

  // Check if noise exceeds threshold
  if (soundValue > noiseThreshold && !alertActive) {
    Serial.println("Noise Detected! Danger!");

    // Reset the green LED if it's still on
    digitalWrite(greenLEDPin, LOW);      // Turn off green LED immediately
    safeDeclared = false;                // Reset safe state

    // Turn on the red LED for alert and record the start time
    digitalWrite(redLEDPin, HIGH);
    redLEDStartTime = millis();  // Mark when red LED was activated
    alertActive = true;          // Set alert state active
  }

  // Handle red LED timeout
  if (alertActive && (millis() - redLEDStartTime >= alertDuration)) {
    // Turn off the red LED after the alert duration
    digitalWrite(redLEDPin, LOW);
    alertActive = false;         // Reset alert state
    greenLEDStartTime = millis(); // Record when the green LED should start
  }

  // If no alert is active, handle the green LED (safe mode)
  if (!alertActive && !safeDeclared) {
    digitalWrite(greenLEDPin, HIGH);  // Turn on the green LED (safe mode)

    // After the green LED duration, turn it off and declare safe
    if (millis() - greenLEDStartTime >= greenLEDDuration) {
      digitalWrite(greenLEDPin, LOW);  // Turn off green LED
      safeDeclared = true;             // Declare the system safe after timeout
      Serial.println("No noise detected. Declaring Safe.");
    }
  }

  // Small delay to prevent flooding the serial monitor
  delay(100);
}

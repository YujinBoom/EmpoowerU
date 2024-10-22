// Pin definitions
const int soundSensorPin = 34; // Analog pin for sound sensor
const int redLEDPin = 26;      // Pin for red LED
const int greenLEDPin = 25;    // Pin for green LED

// Define your threshold value (set this based on your testing from Part 1)
int noiseThreshold = 10;

// Variable to track the green LED blink cycle
int greenBlinkCount = 0;
const int maxGreenBlink = 5; // Number of times to blink the green LED after noise is reduced

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

    // Reset the green LED blink count to 0, so it can blink again once noise subsides
    greenBlinkCount = 0;

    // Display noise status in the serial monitor
    Serial.println("There is noise");

  } else {
    // Noise level below threshold
    Serial.println("Noise Level Below Threshold");

    // Only blink the green LED if it hasn't already blinked for the max times
    if (greenBlinkCount < maxGreenBlink) {
      digitalWrite(greenLEDPin, HIGH); // Turn on green LED
      delay(500);                      // Keep it on for 500ms
      digitalWrite(greenLEDPin, LOW);  // Turn off green LED
      delay(500);                      // Wait 500ms

      // Increment the blink count
      greenBlinkCount++;
    }

    // Display no-noise status in the serial monitor
    if (greenBlinkCount >= maxGreenBlink) {
      Serial.println("Green LED has completed its blink cycle. Now off until noise is detected again.");
    } else {
      Serial.println("There is no noise, green LED blinking...");
    }
  }

  // Delay for readability
  delay(100);
}
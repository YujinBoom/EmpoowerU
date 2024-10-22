// Pin definitions
const int soundSensorPin = 34;  // Analog pin for sound sensor
const int redLEDPin = 13;       // Pin for red LED
const int greenLEDPin = 12;     // Pin for green LED

int peakValue = 0;              // Store the highest (peak) sound value
int threshold = 100;            // Set a threshold for loud sounds (clap)
int silenceTime = 0;            // Counter for silence (no sound)

void setup() {
  // Set LED pins as outputs
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);

  // Start Serial communication
  Serial.begin(115200);
  Serial.println("Sound Sensor Peak Detection with LED Control...");
}

void loop() {
  // Read the current sound sensor value
  int soundValue = analogRead(soundSensorPin);

  // Update the peak value if the current sound value is higher
  if (soundValue > peakValue) {
    peakValue = soundValue;
  }

  // Check if the peak value exceeds the threshold
  if (peakValue > threshold) {
    Serial.println("Loud Sound Detected (Clap)!");
    Serial.print("Peak Value: ");
    Serial.println(peakValue);

    // Turn on the red LED when loud noise (clap) is detected
    digitalWrite(redLEDPin, HIGH);
    digitalWrite(greenLEDPin, LOW);  // Turn off the green LED

    // Reset silence time when sound is detected
    silenceTime = 0;
  } 
  else if (soundValue < threshold && soundValue > 0) {
    // Sound is below the threshold (quiet sound)
    Serial.println("Quiet Sound Detected...");

    // Turn on the green LED for quiet sounds
    digitalWrite(greenLEDPin, HIGH);
    digitalWrite(redLEDPin, LOW);  // Turn off the red LED
  } 
  else {
    // No significant sound detected (no noise)
    silenceTime++;
    if (silenceTime > 50) {  // If no sound for some time, turn off all LEDs
      digitalWrite(redLEDPin, LOW);
      digitalWrite(greenLEDPin, LOW);
      silenceTime = 0;  // Reset silence counter after turning off LEDs
    }
  }

  // Print the current sound sensor value for monitoring
  Serial.print("Current Sound Sensor Value: ");
  Serial.println(soundValue);

  // Reset peak value after every loop cycle
  peakValue = 0;

  // Short delay to allow time for sound peaks (adjust if necessary)
  delay(100);  // Short delay of 100ms for quick sound detection
}

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin definitions
const int soundSensorPin = 34;  // Analog pin for sound sensor
const int redLEDPin = 13;       // Pin for red LED
const int greenLEDPin = 12;     // Pin for green LED

// Define threshold for noise detection
int noiseThreshold = 700;
unsigned long alertDuration = 5000;      // How long to keep LEDs on after noise (milliseconds)
unsigned long greenLEDDuration = 3000;   // How long to keep green LED on after no noise
unsigned long lcdTimeout = 3000;         // How long to show danger/safe message on LCD

// Timing variables
unsigned long redLEDStartTime = 0;
unsigned long greenLEDStartTime = 0;
unsigned long lcdStartTime = 0;
bool alertActive = false;
bool safeDeclared = false;
bool showOnLCD = false;

// LCD setup: Set your I2C address (e.g., 0x27) and 16x2 display size
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Initialize pins
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);

  // Start Serial communication for debugging
  Serial.begin(115200);
  Serial.println("Danger Sense Detector Initialized");

  // Initialize the LCD
  lcd.init();                      // Initialize the LCD
  lcd.backlight();                  // Turn on the backlight
  lcd.setCursor(0, 0);
  lcd.print("System Ready");        // Default message
}

void loop() {
  // Read the sound sensor value
  int soundValue = analogRead(soundSensorPin);
  Serial.print("Sound Sensor Value: ");
  Serial.println(soundValue);

  // Check if noise exceeds threshold
  if (soundValue > noiseThreshold && !alertActive) {
    Serial.println("Noise Detected! Danger!");

    // Reset green LED and LCD if they're still on
    digitalWrite(greenLEDPin, LOW);
    lcd.clear();
    
    // Display "Danger!" on the LCD
    lcd.setCursor(0, 0);
    lcd.print("Danger Detected!");
    lcdStartTime = millis();
    showOnLCD = true;  // Mark LCD to show message

    // Turn on red LED for alert
    digitalWrite(redLEDPin, HIGH);
    redLEDStartTime = millis();  // Mark red LED start time
    alertActive = true;          // Set alert state
    safeDeclared = false;        // Reset safe state
  }

  // Handle red LED timeout
  if (alertActive && (millis() - redLEDStartTime >= alertDuration)) {
    // Turn off red LED after the alert duration
    digitalWrite(redLEDPin, LOW);
    alertActive = false;         // Reset alert state
    greenLEDStartTime = millis(); // Record green LED start time
  }

  // If no alert is active, handle green LED (safe mode)
  if (!alertActive && !safeDeclared) {
    digitalWrite(greenLEDPin, HIGH);  // Turn on green LED

    // Display "Safe" on the LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("All Clear! Safe.");
    lcdStartTime = millis();
    showOnLCD = true;  // Mark LCD to show safe message

    // After green LED duration, turn off green LED and declare safe
    if (millis() - greenLEDStartTime >= greenLEDDuration) {
      digitalWrite(greenLEDPin, LOW);  // Turn off green LED
      safeDeclared = true;             // Declare safe state
      Serial.println("No noise detected. Declaring Safe.");
    }
  }

  // Handle LCD timeout
  if (showOnLCD && (millis() - lcdStartTime >= lcdTimeout)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("System Ready");  // Default message
    showOnLCD = false;          // Stop showing alert/safe messages
  }

  // Small delay to prevent flooding the serial monitor
  delay(100);
}

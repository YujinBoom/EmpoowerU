#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// NOTE: THE BUZZER AND VIBRATION MIGHT NEED A POWER SUPPLY MORE THAN THE 3.3V TO FUNCTION
// CODE DOESNT WORK WELL WITH BUZZER AND VIBRATION MOTOR.
// THE LCD WILL NOT DISPLAY TEXT IF YOU CONNECT IT TO 3.3V IT NEEDS 5V(VIN) TO DISPLAY WELL
// Pin definitions
const int soundSensorPin = 34;  // Analog pin for sound sensor
const int redLEDPin = 13;       // Pin for red LED
const int greenLEDPin = 12;     // Pin for green LED
const int buzzerPin = 25;       // Pin for buzzer
const int vibrationMotorPin = 26; // Pin for vibration motor

// Define threshold for noise detection
int noiseThreshold = 700;
unsigned long alertDuration = 5000;      // How long to keep LEDs, buzzer, and motor on after noise (milliseconds)
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
  pinMode(buzzerPin, OUTPUT);
  pinMode(vibrationMotorPin, OUTPUT);

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

    // Turn on red LED, buzzer, and vibration motor for alert
    digitalWrite(redLEDPin, HIGH);
    digitalWrite(buzzerPin, HIGH);       // Turn on the buzzer
    digitalWrite(vibrationMotorPin, HIGH); // Turn on the vibration motor

    redLEDStartTime = millis();  // Mark start time for alert
    alertActive = true;          // Set alert state
    safeDeclared = false;        // Reset safe state
  }

  // Handle alert timeout (red LED, buzzer, vibration motor)
  if (alertActive && (millis() - redLEDStartTime >= alertDuration)) {
    // Turn off red LED, buzzer, and vibration motor after the alert duration
    digitalWrite(redLEDPin, LOW);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(vibrationMotorPin, LOW);

    alertActive = false;         // Reset alert state
    greenLEDStartTime = millis(); // Record green LED start time
  }

  // If no alert is active and safe has not yet been declared
  if (!alertActive && !safeDeclared) {
    digitalWrite(greenLEDPin, HIGH);  // Turn on green LED

    // Display "Safe" on the LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("All Clear! Safe.");
    lcdStartTime = millis();   // Start the timeout timer for the LCD
    showOnLCD = true;          // Mark LCD to show safe message

    // After green LED duration, turn off green LED and declare safe
    if (millis() - greenLEDStartTime >= greenLEDDuration) {
      digitalWrite(greenLEDPin, LOW);  // Turn off green LED
      safeDeclared = true;             // Declare safe state
      Serial.println("No noise detected. Declaring Safe.");
    }
  }

  // Handle LCD timeout after showing danger or safe message
  if (showOnLCD && (millis() - lcdStartTime >= lcdTimeout)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("System Ready");  // Reset to default message
    showOnLCD = false;          // Reset LCD state
  }

  // Small delay to prevent flooding the serial monitor
  delay(100);
}

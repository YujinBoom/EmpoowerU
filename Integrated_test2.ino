#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DFRobotDFPlayerMini.h>

// Emergency Alert System Pins
const int soundSensorPin = 34;  // Analog pin for sound sensor
const int redLEDPin = 13;       // Pin for red LED
const int greenLEDPin = 12;     // Pin for green LED

// DFPlayer Mini Pin definitions for RX and TX
#define NEW_RX_PIN 27  // New RX Pin (connect to DFPlayer TX)
#define NEW_TX_PIN 26  // New TX Pin (connect to DFPlayer RX)

// Pin definitions for DFPlayer push buttons
#define BUTTON_PREVIOUS 32
#define BUTTON_NEXT 33
#define BUTTON_PAUSE 25

// Sound Detection and Timing variables
int noiseThreshold = 700;
unsigned long alertDuration = 5000;      // How long to keep red LED on after noise
unsigned long greenLEDDuration = 3000;   // How long to keep green LED on after no noise
unsigned long lcdTimeout = 3000;         // How long to show danger/safe message on LCD
unsigned long redLEDStartTime = 0;
unsigned long greenLEDStartTime = 0;
unsigned long lcdStartTime = 0;
bool alertActive = false;
bool safeDeclared = false;
bool showOnLCD = false;

// DFPlayer Variables
HardwareSerial mySerial(2);  // UART2 for DFPlayer Mini
DFRobotDFPlayerMini myDFPlayer;
bool isPlaying = false;

// Debounce timings for buttons
unsigned long debounceDelay = 300;
unsigned long lastButtonPressTime = 0;

// LCD setup (16x2 display size and I2C address 0x27)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Initialize serial monitor for debugging
  Serial.begin(115200);

  // Initialize pins for Emergency Alert System
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  
  // Initialize DFPlayer Mini with hardware serial
  mySerial.begin(9600, SERIAL_8N1, NEW_RX_PIN, NEW_TX_PIN);
  
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("DFPlayer Mini initialization failed!");
    while (true);  // Stop if DFPlayer Mini doesn't initialize
  }

  Serial.println("DFPlayer Mini online.");
  
  // Set volume level for DFPlayer Mini (0 to 30)
  myDFPlayer.volume(20);
  delay(500);

  // Set button pin modes with internal pull-up resistors
  pinMode(BUTTON_PREVIOUS, INPUT_PULLUP);
  pinMode(BUTTON_NEXT, INPUT_PULLUP);
  pinMode(BUTTON_PAUSE, INPUT_PULLUP);
}

void loop() {
  // Independent Emergency Alert System
  emergencyAlertLoop();

  // Independent DFPlayer Mini Playback Feature
  playbackLoop();
}

// Emergency Alert System Logic
void emergencyAlertLoop() {
  int soundValue = analogRead(soundSensorPin);
  Serial.print("Sound Sensor Value: ");
  Serial.println(soundValue);

  // Check if noise exceeds the threshold and trigger danger alert
  if (soundValue > noiseThreshold && !alertActive) {
    Serial.println("Noise Detected! Danger!");

    // Reset green LED and LCD
    digitalWrite(greenLEDPin, LOW);
    lcd.clear();
    
    // Display "Danger!" on LCD
    lcd.setCursor(0, 0);
    lcd.print("Danger Detected!");
    lcdStartTime = millis();
    showOnLCD = true;

    // Turn on red LED for danger alert
    digitalWrite(redLEDPin, HIGH);
    redLEDStartTime = millis();
    alertActive = true;
    safeDeclared = false;
  }

  // Handle red LED timeout
  if (alertActive && (millis() - redLEDStartTime >= alertDuration)) {
    digitalWrite(redLEDPin, LOW);
    alertActive = false;
    greenLEDStartTime = millis();
  }

  // Handle green LED and safe mode
  if (!alertActive && !safeDeclared) {
    digitalWrite(greenLEDPin, HIGH);
    
    // Display "Safe" on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("All Clear! Safe.");
    lcdStartTime = millis();
    showOnLCD = true;

    // After green LED duration, turn it off and declare safe state
    if (millis() - greenLEDStartTime >= greenLEDDuration) {
      digitalWrite(greenLEDPin, LOW);
      safeDeclared = true;
      Serial.println("No noise detected. Declaring Safe.");
    }
  }

  // Handle LCD timeout
  if (showOnLCD && (millis() - lcdStartTime >= lcdTimeout)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("System Ready");
    showOnLCD = false;
  }
}

// DFPlayer Mini Playback Feature Logic
void playbackLoop() {
  unsigned long currentTime = millis();

  // Check if "Previous" button is pressed with debounce
  if (digitalRead(BUTTON_PREVIOUS) == LOW && currentTime - lastButtonPressTime >= debounceDelay) {
    Serial.println("Previous button pressed");
    myDFPlayer.previous();  // Play previous track
    lastButtonPressTime = currentTime;
  }

  // Check if "Next" button is pressed with debounce
  if (digitalRead(BUTTON_NEXT) == LOW && currentTime - lastButtonPressTime >= debounceDelay) {
    Serial.println("Next button pressed");
    myDFPlayer.next();  // Play next track
    lastButtonPressTime = currentTime;
  }

  // Check if "Pause/Play" button is pressed with debounce
  if (digitalRead(BUTTON_PAUSE) == LOW && currentTime - lastButtonPressTime >= debounceDelay) {
    Serial.println("Pause/Play button pressed");

    if (isPlaying) {
      myDFPlayer.pause();  // Pause the current track
      isPlaying = false;
    } else {
      myDFPlayer.start();  // Resume playing the current track
      isPlaying = true;
    }
    lastButtonPressTime = currentTime;
  }
}

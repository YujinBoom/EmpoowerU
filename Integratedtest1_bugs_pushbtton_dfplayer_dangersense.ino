#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DFRobotDFPlayerMini.h>

// Emergency Alert System Pins
const int soundSensorPin = 34;  
const int redLEDPin = 13;
const int greenLEDPin = 12;

// Playback Feature Pins
#define BUTTON_PREVIOUS 32
#define BUTTON_NEXT 33
#define BUTTON_PAUSE 25
#define NEW_RX_PIN 27  
#define NEW_TX_PIN 26  

// Timers for Emergency System
unsigned long alertDuration = 5000;
unsigned long greenLEDDuration = 3000;
unsigned long lcdTimeout = 3000;
int noiseThreshold = 700;

// DFPlayer Mini Setup
HardwareSerial mySerial(2);  
DFRobotDFPlayerMini myDFPlayer;
bool isPlaying = false;

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Setup for Emergency Alert
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(soundSensorPin, INPUT);
  lcd.init();
  lcd.backlight();
  
  // Setup for DFPlayer Mini
  mySerial.begin(9600, SERIAL_8N1, NEW_RX_PIN, NEW_TX_PIN);
  myDFPlayer.begin(mySerial);
  myDFPlayer.volume(20);
  
  pinMode(BUTTON_PREVIOUS, INPUT_PULLUP);
  pinMode(BUTTON_NEXT, INPUT_PULLUP);
  pinMode(BUTTON_PAUSE, INPUT_PULLUP);
}

void loop() {
  // Independent Emergency Alert System
  emergencyAlertLoop();

  // Independent Playback Feature
  playbackLoop();
}

// Emergency Alert System
void emergencyAlertLoop() {
  int soundValue = analogRead(soundSensorPin);
  
  if (soundValue > noiseThreshold) {
    digitalWrite(redLEDPin, HIGH);
    digitalWrite(greenLEDPin, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Danger Detected!");
    delay(alertDuration);
  } else {
    digitalWrite(redLEDPin, LOW);
    digitalWrite(greenLEDPin, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("All Clear! Safe.");
    delay(greenLEDDuration);
  }
}

// Playback Feature
void playbackLoop() {
  if (digitalRead(BUTTON_PREVIOUS) == LOW) {
    myDFPlayer.previous();
    delay(300); 
  }

  if (digitalRead(BUTTON_NEXT) == LOW) {
    myDFPlayer.next();
    delay(300);
  }

  if (digitalRead(BUTTON_PAUSE) == LOW) {
    if (isPlaying) {
      myDFPlayer.pause();
      isPlaying = false;
    } else {
      myDFPlayer.start();
      isPlaying = true;
    }
    delay(300);
  }
}

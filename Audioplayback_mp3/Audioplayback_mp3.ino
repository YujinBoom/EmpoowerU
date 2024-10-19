#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

// Pin definitions for push buttons
#define BUTTON_PREVIOUS 32
#define BUTTON_NEXT 33
#define BUTTON_PAUSE 25

// Use HardwareSerial on ESP32 (UART1 on pins TX=17, RX=16)
HardwareSerial mySerial(1); // UART1 is available on ESP32

DFRobotDFPlayerMini myDFPlayer;

bool isPlaying = false;

void setup() {
  // Initialize serial monitor
  Serial.begin(115200);
  
  // Initialize hardware serial for DFPlayer (9600 baud rate, RX=16, TX=17)
  mySerial.begin(9600, SERIAL_8N1, 16, 17);
  
  // Initialize DFPlayer Mini
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("DFPlayer Mini initialization failed!");
    while (true);
  }

  Serial.println("DFPlayer Mini online.");
  
  // Set volume level (0 to 30)
  myDFPlayer.volume(20);
  delay(500);

  // Pin Modes for buttons
  pinMode(BUTTON_PREVIOUS, INPUT_PULLUP);  // Use pull-up resistor
  pinMode(BUTTON_NEXT, INPUT_PULLUP);
  pinMode(BUTTON_PAUSE, INPUT_PULLUP);
}

void loop() {
  // Check if "Previous" button is pressed
  if (digitalRead(BUTTON_PREVIOUS) == LOW) {
    Serial.println("Previous button pressed");
    myDFPlayer.previous();  // Play previous track
    delay(300);  // Debounce delay
  }

  // Check if "Next" button is pressed
  if (digitalRead(BUTTON_NEXT) == LOW) {
    Serial.println("Next button pressed");
    myDFPlayer.next();  // Play next track
    delay(300);  // Debounce delay
  }

  // Check if "Pause/Play" button is pressed
  if (digitalRead(BUTTON_PAUSE) == LOW) {
    Serial.println("Pause/Play button pressed");

    if (isPlaying) {
      myDFPlayer.pause();  // Pause the current track
      isPlaying = false;
    } else {
      myDFPlayer.start();  // Resume playing the current track
      isPlaying = true;
    }
    delay(300);  // Debounce delay
  }
}

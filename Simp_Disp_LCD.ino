#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Use the correct I2C address

void setup() {
  Serial.begin(115200);
  lcd.init();               // Initialize the LCD
  lcd.backlight();          // Turn on the backlight
  lcd.setCursor(0, 0);      // Set cursor to the top-left
  lcd.print("Test");        // Print "Test" to check display
}

void loop() {
  // Empty loop
}

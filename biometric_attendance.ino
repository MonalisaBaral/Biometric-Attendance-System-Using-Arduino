#include "Adafruit_Fingerprint.h" // Fingerprint library
#include <EEPROM.h>               // For storing data
#include <LiquidCrystal.h>        // LCD library
#include <SoftwareSerial.h>       // For communication with fingerprint sensor
#include <Wire.h>                 // I2C communication
#include "RTClib.h"               // RTC library for DS3231 module

// Pin assignments
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
SoftwareSerial fingerPrint(2, 3); // TX/RX for R305/R307 Fingerprint sensor
RTC_DS3231 rtc;                   // Real-Time Clock (RTC)

uint8_t id;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerPrint);

// Define push button and other pin configurations
#define register_back 14
#define delete_ok 15
#define forward 16
#define reverse 17
#define match 5
#define indFinger 7
#define buzzer 5

#define records 10 // Maximum number of users

// Variables for user attendance counts
int user1, user2, user3, user4, user5;
DateTime now;

void setup() {
  delay(1000);
  lcd.begin(16, 2);
  Serial.begin(9600);

  // Initialize pins
  pinMode(register_back, INPUT_PULLUP);
  pinMode(forward, INPUT_PULLUP);
  pinMode(reverse, INPUT_PULLUP);
  pinMode(delete_ok, INPUT_PULLUP);
  pinMode(match, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(indFinger, OUTPUT);
  digitalWrite(buzzer, LOW);

  // Check if the reset button is pressed at startup
  if (digitalRead(register_back) == 0) {
    resetSystem();
  }

  // Initialize the fingerprint sensor
  initializeFingerprintModule();

  // Initialize RTC
  initializeRTC();

  lcd.setCursor(0, 0);
  lcd.print(" Press Match to ");
  lcd.setCursor(0, 1);
  lcd.print(" Start System");
  delay(3000);

  // Read the attendance record from EEPROM
  user1 = EEPROM.read(1000);
  user2 = EEPROM.read(1001);
  user3 = EEPROM.read(1002);
  user4 = EEPROM.read(1003);
  user5 = EEPROM.read(1004);

  digitalWrite(indFinger, HIGH); // Turn on the fingerprint indicator LED
}

void loop() {
  now = rtc.now(); // Fetch the current time

  // Display current time and date on the LCD
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  displayTime(now);
  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  displayDate(now);
  delay(500);

  // Check for a fingerprint match
  int result = getFingerprintIDez();
  if (result > 0) {
    handleAttendance(result);
  }

  checkKeys(); // Check for button presses
  delay(300);
}

// Function to reset the system and erase all stored data
void resetSystem() {
  lcd.clear();
  lcd.print("Please wait !");
  lcd.setCursor(0, 1);
  lcd.print("Resetting Data");

  for (int i = 1000; i < 1005; i++) {
    EEPROM.write(i, 0); // Reset attendance records
  }
  for (int i = 0; i < 841; i++) {
    EEPROM.write(i, 0xff); // Clear fingerprint storage
  }

  lcd.clear();
  lcd.print("System Reset");
  delay(1000);
}

// Function to initialize the fingerprint sensor
void initializeFingerprintModule() {
  finger.begin(57600);
  lcd.clear();
  lcd.print("Finding Module..");
  delay(2000);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
    lcd.clear();
    lcd.print("Module Found");
    delay(2000);
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    lcd.clear();
    lcd.print("Module Not Found");
    lcd.setCursor(0, 1);
    lcd.print("Check Connections");
    while (1);
  }
}

// Function to initialize the RTC module
void initializeRTC() {
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
  }

  if (rtc.lostPower()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set RTC to current date/time
  }
}

// Display the current time
void displayTime(DateTime now) {
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);
  lcd.print(" ");
}

// Display the current date
void displayDate(DateTime now) {
  lcd.print(now.day(), DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.year(), DEC);
}

// Handle attendance logging
void handleAttendance(int result) {
  digitalWrite(indFinger, LOW); // Turn off LED
  digitalWrite(buzzer, HIGH);   // Sound buzzer for confirmation
  delay(100);
  digitalWrite(buzzer, LOW);

  lcd.clear();
  lcd.print("ID: ");
  lcd.print(result);
  lcd.setCursor(0, 1);
  lcd.print("Please Wait....");
  delay(1000);

  attendance(result); // Log the attendance

  lcd.clear();
  lcd.print("Attendance ");
  lcd.setCursor(0, 1);
  lcd.print("Registered");
  delay(1000);
  digitalWrite(indFinger, HIGH); // Turn on LED again
}

// Core attendance logging function
void attendance(int id) {
  int user = 0;
  int eepLoc = 0;

  switch (id) {
    case 1: eepLoc = 0; user = user1++; break;
    case 2: eepLoc = 210; user = user2++; break;
    case 3: eepLoc = 420; user = user3++; break;
    case 4: eepLoc = 630; user = user4++; break;
    case 5: eepLoc = 840; user = user5++; break;
    default: return;
  }

  int eepIndex = (user * 7) + eepLoc;
  EEPROM.write(eepIndex++, now.hour());
  EEPROM.write(eepIndex++, now.minute());
  EEPROM.write(eepIndex++, now.second());
  EEPROM.write(eepIndex++, now.day());
  EEPROM.write(eepIndex++, now.month());
  EEPROM.write(eepIndex++, now.year() >> 8);
  EEPROM.write(eepIndex++, now.year());

  EEPROM.write(1000, user1);
  EEPROM.write(1001, user2);
  EEPROM.write(1002, user3);
  EEPROM.write(1003, user4);
  EEPROM.write(1004, user5);
}

// Check for button presses
void checkKeys() {
  if (digitalRead(register_back) == 0) {
    lcd.clear();
    lcd.print("Please Wait");
    delay(1000);
    while (digitalRead(register_back) == 0);
    enrollFingerprint(); // Enroll a new fingerprint
  } else if (digitalRead(delete_ok) == 0) {
    lcd.clear();
    lcd.print("Please Wait");
    delay(1000);
    deleteFingerprint(); // Delete a fingerprint
  }
}

// Fingerprint enrollment process
void enrollFingerprint() {
  int count = 1;
  lcd.clear();
  lcd.print("Enter Finger ID:");

  while (1) {
    lcd.setCursor(0, 1);
    lcd.print(count);

    if (digitalRead(forward) == 0) {
      count++;
      if (count > records) count = 1;
      delay(500);
    } else if (digitalRead(reverse) == 0) {
      count--;
      if (count < 1) count = records;
      delay(500);
    } else if (digitalRead(delete_ok) == 0) {
      id = count;
      getFingerprintEnroll(); // Save the fingerprint
      for (int i = 0; i < records; i++) {
        if (EEPROM.read(i) != 0xff) {
          EEPROM.write(i, id);
          break;
        }
      }
      return;
    } else if (digitalRead(register_back) == 0) {
      return;
    }
  }
}

// Function to delete a fingerprint
void deleteFingerprint() {
  int count = 1;
  lcd.clear();
  lcd.print("Enter Finger ID");

  while (1) {
    lcd.setCursor(0, 1);
    lcd.print(count);

    if (digitalRead(forward) == 0) {
      count++;
      if (count > records) count = 1;
      delay(500);
    } else if (digitalRead(reverse) == 0) {
      count--;
      if (count < 1) count = records;
      delay(500);
    } else if (digitalRead(delete_ok) == 0) {
      id = count;
      deleteStoredFingerprint(id); // Delete the fingerprint
      for (int i = 0; i < records; i++) {
        if (EEPROM.read(i) == id) {
          EEPROM.write(i, 0xff);
          break;
        }
      }
      return;
    } else if (digitalRead(register_back) == 0) {
      return;
    }
  }
}

// Helper functions for fingerprint enrollment, deletion, and searching

// Get the fingerprint ID quickly
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) return -1;

  // Found a match!
  return finger.fingerID;
}

// Enroll a fingerprint in the sensor
void getFingerprintEnroll() {
  lcd.clear();
  lcd.print("Waiting for finger");
  while (finger.getImage() != FINGERPRINT_OK);
  finger.image2Tz(1);
  
  lcd.clear();
  lcd.print("Remove finger");
  while (finger.getImage() != FINGERPRINT_NOFINGER);

  lcd.clear();
  lcd.print("Place same finger");
  while (finger.getImage() != FINGERPRINT_OK);
  finger.image2Tz(2);
  
  if (finger.createModel() != FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Error in creating");
    return;
  }

  if (finger.storeModel(id) != FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Error storing");
    return;
  }

  lcd.clear();
  lcd.print("Enrolled!");
}

// Delete a fingerprint from the sensor
void deleteStoredFingerprint(uint8_t id) {
  if (finger.deleteModel(id) == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Deleted!");
  } else {
    lcd.clear();
    lcd.print("Error deleting");
  }
}

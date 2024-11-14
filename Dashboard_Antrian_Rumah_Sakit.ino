#include "SoftwareSerial.h"
#include <LiquidCrystal_I2C.h>

// Define the software serial pins for DFPlayer
SoftwareSerial mySerial(10, 11); // RX, TX

// Create DFPlayer Mini object
#define Start_Byte 0x7E
#define Version_Byte 0xFF
#define Command_Length 0x06
#define End_Byte 0xEF
#define Acknowledge 0x00 // Returns info with command 0x41 [0x01: info, 0x00: no info]
#define ACTIVATED LOW

// LCD configuration
int lcdColumns = 16;
int lcdRows = 4;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

// Button pins
const int buttonNext = 2;
const int buttonPrevious = 4;
const int buttonPin = 3; // LCD backlight control button
const int buttonAntrian = 5;

// MP3 player state
boolean isPlaying = false; // Initial state for DFPlayer
int currentTrack = 1;

// LCD display state
int queueNumber = 0; // Initial queue number is 0 so it starts at 1 when pressed
int queueNumber1 = 10; // Initial status of current queue
int buttonState = 0;
int lastButtonState = 0;

void playFirst();
void play();
void playNext();
void playPrevious();
void setVolume(int volume);
void execute_CMD(byte CMD, byte Par1, byte Par2);
void displayInitialMessage();
void playQueueNumber(int queueNum);

void setup() {
    // Initialize serial for DFPlayer
    mySerial.begin(9600);
    Serial.begin(9600);

    // Initialize buttons
    pinMode(buttonNext, INPUT);
    digitalWrite(buttonNext, HIGH);
    pinMode(buttonPrevious, INPUT);
    digitalWrite(buttonPrevious, HIGH);
    pinMode(buttonPin, INPUT);
    digitalWrite(buttonPin, HIGH);
    pinMode(buttonAntrian, INPUT);
    digitalWrite(buttonAntrian, HIGH);

    // Initialize LCD
    lcd.init();
    lcd.backlight();

    // Display initial message
    lcd.print("Ambil Antrian");
}

void loop() {
    // Handle MP3 player button presses
    if (digitalRead(buttonNext) == ACTIVATED) {
        if (isPlaying) {
            playQueueNumber(queueNumber + 2);
        }
    }

    if (digitalRead(buttonPrevious) == ACTIVATED) {
        if (isPlaying) {
            playQueueNumber(queueNumber1 + 2);
        }
    }

    // Handle LCD backlight control button (buttonAntrian)
    buttonState = digitalRead(buttonAntrian);
    if (buttonState != lastButtonState) {
        if (buttonState == LOW) {
            // Increment queue number
            queueNumber++;

            // Reset queue number to 1 if it exceeds 10
            if (queueNumber > 10) {
                queueNumber = 1;
            }

            // Turn on LCD backlight and display current queue number
            lcd.backlight();
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Selamat Datang");
            lcd.setCursor(0, 1);
            lcd.print("Nomor Antrian");
            lcd.setCursor(0, 2);
            lcd.print("Anda Adalah");
            lcd.setCursor(0, 3);
            lcd.print("Nomor: ");
            lcd.print(queueNumber);

            // Delay to prevent bouncing
            delay(5000);

            // Clear only the fourth row
            lcd.setCursor(0, 3);
            lcd.print("              "); // Clear the line

            // Turn off LCD backlight and display message
            lcd.backlight();
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Tekan Tombol");
            lcd.setCursor(0, 1);
            lcd.print("Untuk Menyalakan");
            lcd.setCursor(0, 2);
            lcd.print("Layar");
        }

        lastButtonState = buttonState;
    }

    // Handle LCD backlight control button (buttonPin)
    buttonState = digitalRead(buttonPin);
    if (buttonState != lastButtonState) {
        if (buttonState == LOW) {
            if (queueNumber == 1) {
                queueNumber1 = queueNumber1;
            } else if (queueNumber > 1 && queueNumber <= 10) {
                queueNumber1 = queueNumber - 1;
            } else if (queueNumber > 10) {
                queueNumber1 = 1;
            }

            // Turn on LCD backlight and display the current queue number
            lcd.backlight();
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Nomor Antrian");
            lcd.setCursor(0, 1);
            lcd.print("Yang Dilayani");
            lcd.setCursor(0, 2);
            lcd.print("Saat Ini");
            lcd.setCursor(0, 3);
            lcd.print("Nomor: ");
            lcd.print(queueNumber1);

            // Delay to prevent bouncing
            delay(5000);

            // Clear only the fourth row
            lcd.setCursor(0, 3);
            lcd.print("              "); // Clear the line

            // Turn off LCD backlight and display message
            lcd.backlight();
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Tekan Tombol");
            lcd.setCursor(0, 1);
            lcd.print("Untuk Menyalakan");
            lcd.setCursor(0, 2);
            lcd.print("Layar");
        }

        lastButtonState = buttonState;
    }
}

void execute_CMD(byte CMD, byte Par1, byte Par2) {
    // Calculate the checksum (2 bytes)
    word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);

    // Build the command line
    byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge, Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte };

    // Send the command line to the module
    for (byte k = 0; k < 10; k++) {
        mySerial.write(Command_line[k]);
    }
}

void playQueueNumber(int queueNum) {
    execute_CMD(0x03, 0, queueNum); // Command to play track based on queue number
    delay(6000);
}

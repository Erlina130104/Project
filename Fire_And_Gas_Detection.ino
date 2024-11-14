#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int sensorApiPin = A0;
const int mq2Pin = A1;
const int buzzerPin = 11;
const int ledApiPin = 9;  
const int ledGasPin = 6;  

const int nilaiAmbangApi = 100;  
const int nilaiAmbangGas = 500; 

const int serialInterval = 1000;
unsigned long previousSerialMillis = 0;

void setup() {
  Serial.begin(9600);
  pinMode(sensorApiPin, INPUT);
  pinMode(mq2Pin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledApiPin, OUTPUT);  
  pinMode(ledGasPin, OUTPUT);  
  lcd.init();
  lcd.backlight();
}

void loop() {
  int nilaiSensorApi = analogRead(sensorApiPin);
  int nilaiSensorGas = analogRead(mq2Pin);

  lcd.setCursor(0, 0);
  if (nilaiSensorApi < nilaiAmbangApi) {
    lcd.print("deteksi ada Api!");
    digitalWrite(buzzerPin, HIGH);
    analogWrite(ledApiPin, 255);   
    digitalWrite(ledGasPin, LOW);  
  } else if (nilaiSensorGas > nilaiAmbangGas) {
    lcd.print("deteksi ada Gas!");
    digitalWrite(buzzerPin, HIGH);
    analogWrite(ledGasPin, 255);   
    digitalWrite(ledApiPin, LOW);  
  } else {
    lcd.print("Tidak ada bahaya");
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledApiPin, LOW);  
    digitalWrite(ledGasPin, LOW); 
  }

  // Mengirim data sensor ke serial monitor setiap interval tertentu
  unsigned long currentMillis = millis();
  if (currentMillis - previousSerialMillis >= serialInterval) {
    previousSerialMillis = currentMillis;
    Serial.print("Sensor Api: ");
    Serial.print(nilaiSensorApi);
    Serial.print(" - Sensor Gas: ");
    Serial.println(nilaiSensorGas);
  }

  delay(1000);
}
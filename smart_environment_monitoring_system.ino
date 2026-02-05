#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// I2C LCD configuration
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin definitions
const int trigPin = 9;
const int echoPin = 10;
const int ledPin = 7;
const int buzzerPin = 6;
const int tempPin = A0;  // LM35 output connected to A0

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Sensor System");
  delay(2000);
  lcd.clear();
}

void loop() {
  // ----- Distance Measurement -----
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;

  // ----- Temperature Measurement -----
  int sensorValue = analogRead(tempPin);
  float voltage = sensorValue * (5.0 / 1023.0);   // Convert ADC to voltage
  float temperature = voltage * 100.0;           // LM35: 10mV per °C

  // ----- Display on Serial Monitor -----
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | Temp: ");
  Serial.print(temperature);
  Serial.println(" C");

  // ----- Display on LCD -----
  lcd.setCursor(0, 0);
  lcd.print("Distance:");
  lcd.setCursor(10, 0);
  lcd.print(distance);
  lcd.print("cm ");

  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.setCursor(6, 1);
  lcd.print(temperature, 1);
  lcd.print((char)223);  // degree symbol
  lcd.print("C ");

  // ----- Alert System -----
  if (distance < 10 && distance > 0) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
    delay(200);
  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
  }

  delay(300);
}

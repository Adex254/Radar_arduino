#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Pins
const int trigPin = 5;
const int echoPin = 6;
const int servoPin = 9;
const int buzzerPin = 8;

// Variables
long duration;
int distance;
Servo radarServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  
  radarServo.attach(servoPin);
  
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("RADAR ONLINE");
  delay(1500);
}

void loop() {
  // Sweep from 15 to 165 degrees
  for (int angle = 15; angle <= 165; angle += 3) {
    scanRadar(angle);
  }
  // Sweep back from 165 to 15 degrees
  for (int angle = 165; angle >= 15; angle -= 3) {
    scanRadar(angle);
  }
}

void scanRadar(int angle) {
  radarServo.write(angle);
  delay(60); // Give motor time to move
  
  // Measure distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  
  // Display data
  lcd.setCursor(0, 0);
  lcd.print("ANG: ");
  lcd.print(angle);
  lcd.print((char)223); // Degree symbol
  lcd.print("   "); 
  
  lcd.setCursor(0, 1);
  lcd.print("DIST: ");
  if (distance > 0 && distance < 100) {
    lcd.print(distance);
    lcd.print("cm   ");
    
    // Warning trigger if an object is closer than 20cm
    if (distance < 20) {
      digitalWrite(buzzerPin, HIGH);
      delay(20);
      digitalWrite(buzzerPin, LOW);
    }
  } else {
    lcd.print("CLEAR   ");
  }
}

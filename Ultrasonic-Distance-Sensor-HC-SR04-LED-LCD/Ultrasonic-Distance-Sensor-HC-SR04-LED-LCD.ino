#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// defines pins numbers
const int trigPin = 9;
const int echoPin = 10;
const int ledPin = 8;

// defines variables
long duration;
float distance;
int safetyDistance = 10;


void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600); // Starts the serial communication
}

void loop() {
  ultra();
  runLCD(distance);
  delay(200);
}


void runLCD(int dist) {
  lcd.begin(16, 2);
  
  lcd.setCursor(0,0);
  lcd.print("Sensor Distancia");
  
  lcd.setCursor(0,1);
  lcd.print("Dt: " + String(dist) + "cm" + " " + String(dist*0.01) + "m");
}

void ultra() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  distance= duration*0.0340/2;
  
  if (distance <= safetyDistance){
    digitalWrite(ledPin, HIGH);
  }
  else{
    digitalWrite(ledPin, LOW);
  }
}

void timer() {
  //delay(hrs * 3600000);
  //delay(hrs * 1000);
}

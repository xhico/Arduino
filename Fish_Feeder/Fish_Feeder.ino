#include <Servo.h>
//#include <LiquidCrystal.h>

Servo myservo;
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int const servoPin = 8;
int const hrs = 36;
//int numbFeed = 0;

void setup() {
}

void loop() {
  //runLCD(numbFeed);
  runFeeder();
  timer();
  //numbFeed += 1;
}

//void runLCD(int numbFeed) {\
//  lcd.begin(16, 2);
//  
//  lcd.setCursor(3,0);
//  lcd.print("FishFeeder");
//  
//  lcd.setCursor(5,1);
//  lcd.print("Food:" + String(numbFeed));
//}



void runFeeder() {
  int pos = 0;
  int angle = 30;
  int const ms = 5;

  myservo.attach(servoPin);
  
  // goes from 0 degrees to 180 degrees in steps of 1 degree
  for (pos = 0; pos <= angle; pos += 1) {
    myservo.write(pos);
    delay(ms);                       
  }
  
  // goes from 180 degrees to 0 degrees
  for (pos = angle; pos >= 0; pos -= 1) { 
	  myservo.write(pos);
	  delay(ms);
  }
  
  myservo.detach();
}

void timer() {
  delay(hrs * 3600000);
  //delay(hrs * 1000);
}

#include <Servo.h>

Servo myservo;
int servoPin = 8;

void setup() {
  myservo.attach(servoPin);
}


void loop() {
  int pos = 0;
  int angle = 180;
  int const ms = 5;
  
  for (pos = 0; pos <= angle; pos += 1) {
    myservo.write(pos);
    delay(ms);                       
  }

  for (pos = angle; pos >= 0; pos -= 1) { 
    myservo.write(pos);
    delay(ms);
  }
} 

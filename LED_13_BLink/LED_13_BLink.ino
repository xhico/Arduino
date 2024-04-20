#define LED_13 13

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_13, HIGH);
  delay(1000);
  digitalWrite(LED_13, LOW);
  delay(1000);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(32, OUTPUT);
}

void loop() {
  digitalWrite(32, LOW);
  delay(1500);
  digitalWrite(32, HIGH);
  delay(1500);
}

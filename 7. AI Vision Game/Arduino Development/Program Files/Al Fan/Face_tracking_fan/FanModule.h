
int pin1 = 22;
int pin2 = 23;

void FanModule_init() {
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
}

void FanModule_on() {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
}

void FanModule_off() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
}
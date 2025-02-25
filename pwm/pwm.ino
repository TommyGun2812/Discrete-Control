#define pwm1 12 
#define pwm2 13

void setup() {
  // put your setup code here, to run once:
  pinMode(pwm1, OUTPUT);
  pinMode(pwm2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(pwm1,80);
  analogWrite(pwm2,0);
}

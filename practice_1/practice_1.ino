#define EN_A 19
#define EN_B 18

double pos = 0.0;
double timer = 0.0;
double pos_ant = 0.0;
double vel = 0.0;

void IRAM_ATTR int_callback(){
  if(digitalRead(EN_B) == 0){
    pos = pos + 1;
  } else {
    pos = pos - 1;
    }
}
  
void setup() {
  Serial.begin(115200);
  pinMode(EN_A, INPUT);
  pinMode(EN_B, INPUT);
  attachInterrupt(EN_A, int_callback, RISING);
}

void loop() {
  if(micros()-timer >= 100000.0){
    timer = micros();
    vel = 60.0 * 10 * (pos - pos_ant) / (12.0*34.0);
    pos_ant = pos;
    Serial.println(vel);
    }
}

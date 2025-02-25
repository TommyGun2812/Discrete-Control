// Pines ESP
#define EN_A 18
#define EN_B 19

//ENCODER
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

// ganancias del modelo continuo
float kp = 0;
float ki = 0;
float kd = 0;
float T = 0.01; //Periodo de muestreo en segundo

float K1,K2,K3;
float tiempo = 0;
float referencia = 100;
float retro;
float e[3]={0,0,0};//e[0] error actual e[1] erro anterior
float u[2]={0,0};//u[0] salida actual  u[1] salida anterior
int cw = 5;//pines temporales
int ccw= 6;//pines temporales

//Configuracion del PWM
#define PWM_CH 0
#define FREQ 500
#define RES 8

void setup() {
  pinMode(cw,OUTPUT);
  pinMode(ccw,OUTPUT);
  // put your setup code here, to run once:
  K1 = kp + T*ki + kd/T;
  K2 = -kp - 2.0*kd/T;
  K3 = kd/T;

  //Encoder
  pinMode(EN_A, INPUT);
  pinMode(EN_B, INPUT);

  //Configurar el canal de PWM
  ledcSetup(PWM_CH, FREQ, RES);
  ledcAttachPin(8, PWM_CH);//pin asociado al pwm
  //ALTERNATIVO AL PWM
  pinMode(8,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  if(micros()-tiempo>=T*1000000)
  {
    tiempo = micros();
    retro = analogRead(0); // ficticio, momentaneo
    // calcular el error
    e[0]=referencia - retro;
    //calcular ecuación en diferencias o pid
    u[0]=K1*e[0]+K2*e[1]+K3*e[2]+u[1];

    if(u[0]>255)  u[0]=255;
    if(u[0]<-255) u[0]=-255;

    //enviar acción de control
    if(u[0]>=0)
    {
    digitalWrite(cw,HIGH);
    digitalWrite(ccw,LOW);
    //analogWrite(u[0],8); //terminal propuesta
    ledcWrite(PWM_CH, u[0]);
    }
    else
    {
    digitalWrite(cw,LOW);
    digitalWrite(ccw,HIGH);
    ledcWrite(PWM_CH, -u[0]);//analogWrite(-u[0],8); //terminal propuesta      
      }

    // Recorrer las muestras para actualizar
    e[2]=e[1];
    e[1]=e[0];
    u[1]=u[0];
    }


}

#define EN_A 19
#define EN_B 18
#define pi 3.1416

// ganancias del modelo continuo
float kp = 5.933;
float ki = 6.642;
float kd = 1.325;
float T = 0.05; //Periodo de muestreo en segundo

float K1,K2,K3;
float tiempo = 0;
float referencia = 6.28;
float retro;
float e[3]={0,0,0};//e[0] error actual e[1] erro anterior
float u[2]={0,0};//u[0] salida actual  u[1] salida anterior
int cw = 12;//pines temporales
int ccw= 13;//pines temporales

///////////////////////////////////
// Encoder
double pos = 0; //almacena las cuentas del enc
double pos_ant = 0;
double vel;

void IRAM_ATTR int_callback()
{ 
  if(digitalRead(EN_B)==0)
  {
    pos = pos + 1;
    }
   else
   {
    pos = pos - 1;
    }
  }




void setup() {
  Serial.begin(115200);
  pinMode(cw,OUTPUT);
  pinMode(ccw,OUTPUT);
  // put your setup code here, to run once:
  K1 = kp + T*ki + kd/T;
  K2 = -kp - 2.0*kd/T;
  K3 = kd/T;

  // Encoder
  pinMode(EN_A, INPUT);
  pinMode(EN_B, INPUT);
  attachInterrupt(EN_A, int_callback, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:

  if(micros()-tiempo>=T*1000000)
  {
    tiempo = micros();
    //retro = analogRead(0); // ficticio, momentaneo
    retro = 60.0*20.0*(pos - pos_ant)/(12.0*34.0);
    retro = retro*2*pi/60.0;
    pos_ant = pos;
    // calcular el error
    e[0]=referencia - retro;
    //calcular ecuación en diferencias o pid
    u[0]=K1*e[0]+K2*e[1]+K3*e[2]+u[1];

    if(u[0]>255)  u[0]=255;
    if(u[0]<-255) u[0]=-255;

    //u[0] = 100;
    //enviar acción de control
    if(u[0]>=0)
    {
      analogWrite(cw,u[0]);
      analogWrite(ccw,0);
    }
    else
    {
      analogWrite(cw,0);
      analogWrite(ccw,-u[0]);
      }

    // Recorrer las muestras para actualizar
    e[2]=e[1];
    e[1]=e[0];
    u[1]=u[0];
    
    Serial.print(referencia);
    Serial.print("       ");
    Serial.println(retro); //es la velocidad medida
    }


}


String mensaje;
int indexa = 0;
void serialEvent()
{             
    char ch = Serial.read();   
    mensaje.concat(ch); 
    if(mensaje.charAt(indexa) == '\n' && mensaje.charAt(indexa-1) == '\r')
    {
      indexa = 0;
      referencia = mensaje.toFloat();
      mensaje = "";
      }   
    else{
    indexa++;
    }
}

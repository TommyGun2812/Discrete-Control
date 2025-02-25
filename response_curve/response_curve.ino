#define EN_A 18
#define EN_B 19


float T = 0.05; //Periodo de muestreo en segundo


float tiempo = 0;
float referencia = 0;
float retro;
float e[3]={0,0,0};//e[0] error actual e[1] erro anterior
float u[2]={0,0};//u[0] salida actual  u[1] salida anterior
int cw = 5;//pines temporales
int ccw= 6;//pines temporales

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
    analogWrite(cw,referencia);
    //retro = analogRead(0); // ficticio, momentaneo
    retro = 0.10472*60.0*20.0*(pos - pos_ant)/(12.0*34.0);//rads
    pos_ant = pos;
    
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

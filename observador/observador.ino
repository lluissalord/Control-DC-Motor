//#include <avr/io.h>
//#include <avr/interrupt.h>
int ENB=11;//as before
int encoderA=2; //connected to Arduino?s ports 2 and 3
int pulseAlength=0;//encoder Chanel B pulse length in microseconds
unsigned long time_bef;
unsigned long time_now_loop;
volatile double v=0;
volatile double v_sum=0;
volatile double v_mean=0;
volatile char count=0;
int count_loop=0;
double u=0;
double pwm=0;
const double K_poly[4]={0.0004087630826036687, -0.141361790772926, 17.097698790218242, -620.9671400241792};
int v_d=0;

int h=8; //Sampling rate
double Nx[2]={1,2.88329060337114};
double Nu=1.00280248926455;
double K[2]={3.324547450664602,  -0.774925825115605};

#define SIZE 3
//Estat de l'observador
double estat[SIZE]={0, 0, 0};
//Per guardar la variable en aquest mateix instant, sino es modifica i no calcula be x2
double estat_bef[SIZE]={0, 0, 0};
double gam[SIZE]={
  0.209498836038743,
  0.674398104977198,
  0};
double phi[SIZE][SIZE]={
{-0.025032383058131,   0.282644568614630,   0.209498836038743},
{-0.070573033383841,   0.789922297691954,   0.674398104977198},
{                 0,                   0,   1.000000000000000}};
double L[SIZE]={
  0.864889914633823,
   2.454431859332966,
   0.426210231983564};

void setup()
{
  pinMode(ENB,OUTPUT); //outputs
  pinMode(encoderA,INPUT); //inputs
  pinMode(encoderA,INPUT_PULLUP);//Enable pull up resistor
  
  //digitalWrite(IN1,HIGH); digitalWrite(IN2,LOW);//setting motorA?s direction--
  //analogWrite(ENA,150);//start driving motorA
  digitalWrite(encoderA,HIGH);//start driving motorA
  
  Serial.begin(115200); Serial.println("Starting data acquisition...");
  while(!Serial){};
  
  attachInterrupt(digitalPinToInterrupt(encoderA),updateStadistic,RISING);
  time_bef=micros();
  
  Serial.println("v_mean;pwm");

  time_now_loop = micros();
  
  /*//Arrancada
  v_d = 150;
  
  pwm=K_poly[0]*v_d*v_d*v_d+K_poly[1]*v_d*v_d+K_poly[2]*v_d+K_poly[3];

  //Rang de PWM
  if(pwm>255){
    pwm=255;
  }else if (pwm<0){
    pwm=0;
  }
  
  analogWrite(ENA,pwm);
  
  delay(3000); */ 
}

void loop() {
  Serial.print(estat[0]);
  Serial.print(";");
  Serial.print(estat[1]);
  Serial.print(";");
  Serial.print(estat[2]);
  Serial.print(";");
  
  if(count_loop==80){
    v_d=120;
  }else if (count_loop==160){
    v_d=160;
    count_loop=0;
  }
  
  //Velocitat maxima del motor
  if(v_d>180){
    v_d=180;
  }

  u=K[0]*(Nx[0]*v_d-estat[0]) + K[1]*(Nx[1]*v_d-estat[1]) - estat[2];
  u=u+Nu*v_d;
  
  //Actualitzacio estats
  char i=0;
  while(i<SIZE){
    estat_bef[i]=estat[i];
    i++;
  }

  estat[0]=gam[0]*u + L[0]*(v_mean-estat_bef[0]) + estat_bef[0]*phi[0][0] + estat_bef[1]*phi[0][1] + estat_bef[2]*phi[0][2];
  estat[1]=gam[1]*u + L[1]*(v_mean-estat_bef[0]) + estat_bef[0]*phi[1][0] + estat_bef[1]*phi[1][1] + estat_bef[2]*phi[1][2];
  estat[2]=           L[2]*(v_mean-estat_bef[0]) + estat_bef[2];

  pwm=K_poly[0]*u*u*u+K_poly[1]*u*u+K_poly[2]*u+K_poly[3];

  //Rang de PWM
  if(pwm>255){
    pwm=255;
  }else if (pwm<0){
    pwm=0;
  }
  
  analogWrite(ENB,pwm);

  unsigned long time_bef_loop = time_now_loop;
  time_now_loop = micros();
  
  Serial.print(v_mean);
  Serial.print(";");
  Serial.print(v_d);
  Serial.print(";");
  Serial.print(u);
  Serial.print(";");
  Serial.print(pwm);
  Serial.print(";");
  Serial.println(time_now_loop-time_bef_loop);
  
  count_loop++;
  delay(h);
}

//360 pulses total/revolution i per tant 180 pulses per encoder/revolution
void updateStadistic(){
  if (count==2){
    unsigned long time_now = micros();
    v=2*1000000/((time_now-time_bef));
    time_bef=time_now;
    v_mean=(v_sum+v)/3;
    v_sum=0;
    count=0;
  }
  v_sum=v_sum+v;
  count++;
}

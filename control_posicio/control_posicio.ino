//#include <avr/io.h>
//#include <avr/interrupt.h>
int DIRB=13;
int ENB=11;//as before
int encoderA=2; //connected to Arduino?s ports 2 and 3
int encoderB=3; //connected to Arduino?s ports 2 and 3
volatile double count2pos = 3.63636363;//240/66  //360/(360*30);
volatile long count_pulses = 0;
volatile int dir = 1;
volatile int encA = 1;
volatile int encB = 1;
int count_loop=0;
double u=0;
double pwm=0;
const double K_poly[4]={0.0004087630826036687, -0.141361790772926, 17.097698790218242, -620.9671400241792};
int pos_d=0;

#define SIZE 3
int h=8; //Sampling rate
//double Nx[SIZE]={0,100,288.329060337114};
//double Nu=100.2802;
double Nx[SIZE]={1,0,0};
double Nu=0;
double K[SIZE]={2.892844676888288,  18.426678414296934,  -6.566448515378249};
//double K[SIZE]={146.2610011282936,   1.0877332456609,   0.7444195612642};

//Estat de l'observador
double estat[SIZE]={0, 0, 0};
//Per guardar la variable en aquest mateix instant, sino es modifica i no calcula be x2
double estat_bef[SIZE]={0, 0, 0};
double gam[SIZE]={
  0.000976146135944,
  0.209498836038743,
  0.674398104977198};
double phi[SIZE][SIZE]={
{1.000000000000000,   0.000940994645850,   0.002802396529054},
{                0,  -0.025032383058131,   0.282644568614630},
{                0,  -0.070573033383841,   0.789922297691954}};
double L[SIZE]={
  1.7648899146338,
  66.6855922348154,
  186.3163702254405};

int dirrec = 1;
void setup()
{
  pinMode(ENB,OUTPUT); //outputs
  pinMode(DIRB,OUTPUT);
  pinMode(encoderA,INPUT); //inputs
  pinMode(encoderA,INPUT_PULLUP);//Enable pull up resistor
  pinMode(encoderB,INPUT); 
  pinMode(encoderB,INPUT_PULLUP);//Enable pull up resistor
  
  //digitalWrite(IN1,HIGH); digitalWrite(IN2,LOW);//setting motorA?s direction--
  //analogWrite(ENA,150);//start driving motorA
  digitalWrite(encoderA,HIGH);//start driving motorA
  digitalWrite(encoderB,HIGH);//start driving motorB

  attachInterrupt(digitalPinToInterrupt(encoderA),updateStadisticA,FALLING);
  //attachInterrupt(digitalPinToInterrupt(encoderB),updateStadisticB,FALLING);
  
  Serial.begin(115200); Serial.println("Starting data acquisition...");
  while(!Serial){};
  
  Serial.println("X1;X2;X3;pos;pos_d;u;pwm");

  count_pulses = 0;
  pos_d = 0;
  digitalWrite(DIRB,HIGH);
  dir = 1;
}

void loop() {
  Serial.print(estat[0]);
  Serial.print(";");
  Serial.print(estat[1]);
  Serial.print(";");
  Serial.print(estat[2]);
  Serial.print(";");

  if(count_loop==200){
    pos_d=0;
//    dirrec=-dirrec;
//
//    if(dirrec==1){
//      digitalWrite(DIRB,HIGH);
//    }else{
//      digitalWrite(DIRB,LOW);
//    }
  }else if (count_loop==400){
    pos_d=-100;
    count_loop=0;
//    dirrec=-dirrec;
//
//    if(dirrec==1){
//      digitalWrite(DIRB,HIGH);
//    }else{
//      digitalWrite(DIRB,LOW);
//    }
  }

  double pos = count2pos*count_pulses;
  
  u=K[0]*(Nx[0]*pos_d-estat[0]) + K[1]*(Nx[1]*pos_d-estat[1]) + K[2]*(Nx[2]*pos_d-estat[2]);
  u=u+Nu*pos_d;

  double u_orig = u;
//  if(u < 0){
//    u = -u;
//    digitalWrite(DIRB,LOW);
//  }else{
//    digitalWrite(DIRB,HIGH);
//  }
  if(u < 0){
    u = -u;
    dirrec=-1;
  }else{
    dirrec=1;
  }

  if(dirrec==1){
    digitalWrite(DIRB,HIGH);
  }else{
    digitalWrite(DIRB,LOW);
  }
  double pwm=K_poly[0]*u*u*u+K_poly[1]*u*u+K_poly[2]*u+K_poly[3];

  Serial.print(count_pulses);
  Serial.print(";");  
  Serial.print(pos);
  Serial.print(";");
  Serial.print(pos_d);
  Serial.print(";");
  Serial.print(u_orig);
  Serial.print(";");
  Serial.print(pwm);

  //Rang de PWM
  if(pwm>255){
    pwm=255;
    if(u_orig < 0){
      u_orig = -179.08;
    }else{
      u_orig = 179.08;
    }
  }else if (pwm<0){
    pwm=0;
    u_orig = 0;
  }

  Serial.print(";");
  Serial.print(u);
  Serial.print(";");
  Serial.print(encA);
  Serial.print(";");
  Serial.print(encB);
  Serial.print(";");
  Serial.println(pwm);

  analogWrite(ENB,pwm);
  
  //Actualitzacio estats
  char i=0;
  while(i<SIZE){
    estat_bef[i]=estat[i];
    i++;
  }

  estat[0]=gam[0]*u_orig + L[0]*(pos-estat_bef[0]) + estat_bef[0]*phi[0][0] + estat_bef[1]*phi[0][1] + estat_bef[2]*phi[0][2];
  estat[1]=gam[1]*u_orig + L[1]*(pos-estat_bef[0]) + estat_bef[0]*phi[1][0] + estat_bef[1]*phi[1][1] + estat_bef[2]*phi[1][2];
  estat[2]=gam[2]*u_orig + L[2]*(pos-estat_bef[0]) + estat_bef[0]*phi[2][0] + estat_bef[1]*phi[2][1] + estat_bef[2]*phi[2][2];
  
  count_loop++;
  delay(h);
}

//360 pulses total/revolution i per tant 180 pulses per encoder/revolution
void updateStadisticA(){
  encA = digitalRead(encoderA);
  encB = digitalRead(encoderB);

  if(encA*encB == 0){
    dir = 1;
  }else{
    dir = -1;
  }

  count_pulses = count_pulses + dir;
}

void updateStadisticB(){
  encA = digitalRead(encoderA);
  encB = digitalRead(encoderB);

  if(encA*encB == 0){
    dir = -1;
  }else{
    dir = 1;
  }
  
  count_pulses = count_pulses + dir;
}

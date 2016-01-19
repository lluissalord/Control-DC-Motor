//#include <avr/io.h>
//#include <avr/interrupt.h>
int ENA=3;//as before
int encoderA=2; //connected to Arduino?s ports 2 and 3
int pulseAlength=0;//encoder Chanel B pulse length in microseconds
unsigned long time_bef;
volatile double v=0;
volatile double v_sum=0;
volatile double v_mean=0;
volatile char count=0;
double pwm=0;
double v_mean_tot=0;
const double K_poly[4]={0.0004087630826036687, -0.141361790772926, 17.097698790218242, -620.9671400241792};
int v_d=0;

void setup()
{
  pinMode(ENA,OUTPUT); //outputs
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
}

void loop() {      
  v_d=0;

  while(v_d<180){
    //Velocitat maxima del motor
    if(v_d>180){
      v_d=180;
    }
    
    pwm=K_poly[0]*v_d*v_d*v_d+K_poly[1]*v_d*v_d+K_poly[2]*v_d+K_poly[3];

    //Rang de PWM
    if(pwm>255){
      pwm=255;
    }else if (pwm<0){
      pwm=0;
    }
    analogWrite(ENA,pwm);
    

    //Total time of delay = 1000 + 450
    delay(1000);
    for(int i=0;i<30;i++){
      v_mean_tot=(v_mean_tot*i+v_mean)/(i+1);
      delay(15);
    }
    
    Serial.print(v_mean_tot);
    Serial.print(";");
    Serial.print(v_d);
    Serial.print(";");
    Serial.println(pwm);
    
    v_d++;
  }
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

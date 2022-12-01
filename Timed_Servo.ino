#include <Servo.h>

#define A 4
#define B 5
#define C 10
#define D 9
#define E 8
#define F 6
#define G 7
#define DP 11

#define Segment_Button 3

byte Segment_Button_State;
byte Segment_Button_Previous_State;
int seg[]={A,B,C,D,E,F,G,DP};
unsigned long int current_number;
byte numbers[9][8]{
    {0,1,1,0,0,0,0,0},//1
    {1,1,0,1,1,0,1,0},//2
    {1,1,1,1,0,0,1,0},//3
    {0,1,1,0,0,1,1,0},//4
    {1,0,1,1,0,1,1,0},//5
    {1,0,1,1,1,1,1,0},//6
    {1,1,1,0,0,0,0,0},//7
    {1,1,1,1,1,1,1,0},//8
    {1,1,1,1,0,1,1,0},//9
  };

#define Function_Button 2
#define F_LED A4
#define E_LED A3
#define TEST_LED A5

Servo servo;

byte Button_State;
byte Button_Previous_State;
byte F_LED_State; //Funcitoin LED
byte F_LED_blink;
int F_LED_blinking;
byte E_LED_State; //error LED
byte Servo_State;
unsigned long int Servo_Time;

unsigned long int timestamp;
unsigned long int servo_timestamp; //Check time for switch back servo

void setup() {

  pinMode(seg[0],OUTPUT);
  pinMode(seg[1],OUTPUT);
  pinMode(seg[2],OUTPUT);
  pinMode(seg[3],OUTPUT);
  pinMode(seg[4],OUTPUT);
  pinMode(seg[5],OUTPUT);
  pinMode(seg[6],OUTPUT);
  pinMode(seg[7],OUTPUT);

  current_number=4;
  Segment_Button_State=HIGH;
  Segment_Button_Previous_State=HIGH;

  
  servo.attach(12);
  servo.write(0);
  pinMode(Function_Button,INPUT);
  pinMode(Segment_Button,INPUT);
  pinMode(F_LED,OUTPUT);
  pinMode(E_LED,OUTPUT);
  pinMode(TEST_LED,OUTPUT);
  
  Button_Previous_State=HIGH;
  Button_State=HIGH;
  Servo_State=0;
  F_LED_State=0;
  F_LED_blink=0;
  F_LED_blinking=0;
  E_LED_State=0;
  //Servo_Time1=5000;
  //Servo_Time2=1000;
  Servo_Time=1000;

  analogWrite(F_LED,255);
  analogWrite(E_LED,255);
  analogWrite(TEST_LED,255);
  
  Serial.begin(38400);    //Initializing the serial port with 38400 baud.
  while (!Serial);

  timestamp=millis();
  servo_timestamp=timestamp;
  SetSegment(current_number);
}

void SetSegment(int number){
  int i=0;
  int j=0;
  for(i=0;i<9;i++){
    if(i==number-1){
      for(j=0;j<8;j++){
        digitalWrite(seg[j],numbers[i][j]);
      }
    }
  }
}

void Button_Event(){
  Button_State=digitalRead(Function_Button);
  Segment_Button_State=digitalRead(Segment_Button);
  
  if(Button_Previous_State==HIGH && Button_State==LOW){ //Button pussed
    //F_LED_State=abs(F_LED_State-1); if int
    if(F_LED_State==0){
      F_LED_State=1;
    }else{
      F_LED_State=0;
    }
    delay(20);  //Prell defend
  }

  if(Segment_Button_Previous_State==HIGH && Segment_Button_State==LOW){ //Button pussed
    if(current_number==9){
      current_number=1;
    }else{
      current_number++;
    }
    
    SetSegment(current_number);
    
    delay(20);  //Prell defend
  }
  
  Button_Previous_State=Button_State;
  Segment_Button_Previous_State=Segment_Button_State;
}

void Servo_Event(){
  //Serial.println(timestamp%(current_number*60000));
  if(F_LED_State==1 && (timestamp%(current_number*60000))<100 && Servo_State==0){
    servo.write(90);
    analogWrite(TEST_LED,0);
    Servo_State=1;
    servo_timestamp=timestamp;
  }else if(F_LED_State==1 && Servo_State==1 && (timestamp-servo_timestamp)>Servo_Time){
    servo.write(0);
    analogWrite(TEST_LED,255);
    Servo_State=0;
  }
}

void LED_Event(){
  if(F_LED_State==1){
    if((timestamp%(current_number*60000))>(current_number*60000)-10000){
      if(timestamp%500<10){
        if(F_LED_blinking==0){
          if(F_LED_blink==0){
            analogWrite(F_LED,255);
            F_LED_blink=1;
          }else{
            analogWrite(F_LED,0);
            F_LED_blink=0;
          }
          F_LED_blinking++;
        }
      }else{
        F_LED_blinking=0;  
      }
    }else{
      analogWrite(F_LED,0);
      F_LED_blink=0;
      F_LED_blinking=0;
    }
  }else{
    analogWrite(F_LED,255);
    F_LED_blinking=0;
  }
  
}

void loop() {
  timestamp=millis();
  Button_Event();
  Servo_Event();
  LED_Event();

}


String data;
char d1;
int start = 0;
int steps, test, rounds, CHIPS, ls,xx,mx,yy,my,lx,bx,by,ly,xstep,ystep,sx,sy,rx,ry;

float sp;
float ie;
float ii;
const int dirPin_x = 3;
const int stepPin_x = 2;
const int dirPin_y = 5;
const int stepPin_y = 4;
const int dirPin_z = 7;
const int stepPin_z = 6;

const int S0 = 14;
const int S1 = 15;
const int S2 = 16;

int val1 =0;
int val2 =0;
int val3 =0;
String x="";
String tt="";
String jk="";
String ss="";
String gg="";
String wx="";
String wmx="";
String wy="";
String wmy="";
String wsx="";
String wsy="";

#include <ezButton.h>
ezButton limitSwitch1(A1); //arm
ezButton limitSwitch2(A2); //updown
ezButton limitSwitch3(A0); //tray
const byte limitPin1 = A1; 
const byte limitPin2 = A2; 
const byte limitPin3 = A0; 

#include <AccelStepper.h>
#define motorInterfaceType 1

AccelStepper stp1(motorInterfaceType, stepPin_x, dirPin_x);
AccelStepper stp2(motorInterfaceType, stepPin_y, dirPin_y);
AccelStepper stp3(motorInterfaceType, stepPin_z, dirPin_z);

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100);
  stp1.setMaxSpeed(1000);
	stp1.setSpeed(100);
  //stp1.setAcceleration(1);

  stp2.setMaxSpeed(1000);
	stp2.setSpeed(-100);
 	//stp2.setAcceleration(1);

  stp3.setMaxSpeed(1000);
	stp3.setSpeed(100);
  //stp3.setAcceleration(1);

  limitSwitch1.setDebounceTime(50);
  limitSwitch2.setDebounceTime(50);
  limitSwitch3.setDebounceTime(50);
  
    
}
void runForward(int ry, int rx){
   
   stp3.moveTo(-(ystep)*ry);  //250 is 50mm to get to 00 of graph every 50 is 10mm
   stp3.setSpeed(81);              // should be max possible movement towards the limit switch
   while(stp3.distanceToGo()!=0){ stp3.runSpeedToPosition();}
   
  
  if((ry==my)&&(rx!=mx)){ //even case
 
   delay(1200);
   stp3.moveTo( 0 );  //250 is 50mm to get to 00 of graph every 50 is 10mm
   stp3.setSpeed(150);              // should be max possible movement towards the limit switch
    while(stp3.distanceToGo()!=0){ stp3.runSpeedToPosition();}
   
  }}


void reset(){
   stp1.moveTo( -ii);
   stp1.setSpeed(100); 
   while(stp1.distanceToGo()!=0){ stp1.runSpeedToPosition();}
   ii = ii + xstep;

  
  }


void runBackward(int round){
   stp3.moveTo( 0 );  //250 is 50mm to get to 00 of graph every 50 is 10mm
   stp3.setSpeed(81);              // should be max possible movement towards the limit switch
    while(stp3.distanceToGo()!=0){ stp3.runSpeedToPosition();} 

   if(steps!=round){
    stp1.moveTo( -ie );  //250 is 50mm to get to 00 of graph every 50 is 10mm
    stp1.setSpeed(81);              // should be max possible movement towards the limit switch
    while(stp1.distanceToGo()!=0){ stp1.runSpeedToPosition();}}
   
    delay(20);
    ie = ie + test*2;
  }



void calibrate(int sx,int sy,int xx,int mx, int yy, int my){  //motor calibration
  //test=500/(steps*2);
   
  bx=5*5+sx*5;
  by=5*5+sy*5;
  lx=xx*5;
  ly=yy*5;
  xstep=lx/mx;
  ystep=ly/my;
  ii=xstep/2;
  while( digitalRead( limitPin1 ) == LOW) stp1.runSpeed(); 
  stp1.setCurrentPosition(0);
  stp1.moveTo( -bx-xstep/2-7.5 );  //250 is 50mm to get to 00 of graph every 50 is 10mm
  stp1.setSpeed(100);              // should be max possible movement towards the limit switch
  while(stp1.distanceToGo()!=0){ stp1.runSpeedToPosition();}
  stp1.setCurrentPosition(0);


  while( digitalRead( limitPin2 ) == LOW) stp2.runSpeed();
  stp2.setCurrentPosition(0);
  stp2.moveTo( 5 );  //250 is 50mm to get to 00 of graph every 50 is 10mm
  stp2.setSpeed(100);              // should be max possible movement towards the limit switch
  while(stp2.distanceToGo()!=0){ stp2.runSpeedToPosition();}
  stp2.setCurrentPosition(0);

  
  while( digitalRead( limitPin3 ) == LOW) stp3.runSpeed();
  stp3.setCurrentPosition(0);
  stp3.moveTo( -by-ystep/2-7.5 );  //250 is 50mm to get to 00 of graph every 50 is 10mm
  stp3.setSpeed(100);              // should be max possible movement towards the limit switch
  while(stp3.distanceToGo()!=0){ stp3.runSpeedToPosition();}
  stp3.setCurrentPosition(0);

  
}
void loop(){
//set up instructions and choose what happens
//calibrate(110,90,20,20,60,30);

x = Serial.readStringUntil('\n');

Serial.print(x);


if(x.charAt(0)=='c'){
  wsx=x.substring(3,6);
  Serial.print(wsx);
  sx=wsx.toInt();

  wx=x.substring(7,10);
  Serial.print(wx);
  xx=wx.toInt();

  wmx=x.substring(12,15);
  Serial.print(wmx);
  mx=wmx.toInt();

  wy=x.substring(16,19);
  Serial.print(wy);
  yy=wy.toInt();

  wmy=x.substring(21,24);
  Serial.print(wmy);
  my=wmy.toInt();

  wsy=x.substring(26,29);
  Serial.print(wsx);
  sy=wsy.toInt();
  
  
  calibrate(sx,sy,xx,mx,yy,my);
}
else if(x.charAt(0)=='f'){
  ss="";
  gg="";
  jk="";
  tt=x;
  gg=x.substring(3,5);
  
  ss=tt.substring(3,5);
  jk=x.substring(1,3);
  
  ry=jk.toInt();
  rx=gg.toInt();
  //Serial.print("  "+jk+"   "+x+"   "+gg);
  
  runForward(rx, ry);
  
}
else if(x.charAt(0)=='r'){
  
  reset();
}



delay(10);}

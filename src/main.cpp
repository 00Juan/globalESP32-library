#include <Arduino.h>
#include <EdgeDetector.h>
#include <Timer.h>


using namespace GlobalLibrary;

EdgeDetector bot1(16,EDGE_FALLING,PIN_INPUT_PULLUP,50);
Timer timer1(CYCLIC, 1000);
SoftwarePWM pwm(100, 100); // 1 second HIGH, 0.5 second LOW


bool actEdge=false,actTimer;

int pinLed=19;

void ledControl()
{
  static bool state=false;
  state=!state;
  digitalWrite(pinLed,state);
}


void setup() {
pinMode(pinLed,OUTPUT);
Serial.begin(115200);
timer1.setCallback(ledControl);
}



void loop() {
  bot1.update();
  
  pwm.update(true);

  bool actPWM =pwm.getState();
  static bool cmdLed=bot1.getDigitalRead();
  

  // if(bot1.getEdgeDetected())
  // {
  //   actEdge=!actEdge;
  // }

  //actTimer=timer1.getTimerExpired();
  if(actTimer)
  {
    cmdLed=!cmdLed;
  }

  
  bool dRead=bot1.getDigitalRead();
Serial.println((String)"Dread= "+dRead);
  timer1.update(!dRead);


 // Serial.println((String)"Toggle: "+actEdge+" Dread= "+dRead);
  //Serial.println(pwm.getState());
 // Serial.println(timer1.getElapsedTime());
  //digitalWrite(pinLed,true);
}




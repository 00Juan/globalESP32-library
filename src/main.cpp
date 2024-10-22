#include <Arduino.h>
#include <EdgeDetector.h>
#include <Timer.h>

using namespace GlobalLibrary;

EdgeDetector bot1(16, EDGE_FALLING, PIN_INPUT_PULLUP, 50);
Timer timer1(CYCLIC, 1000);
Timer timerPWM(200, 1000); // 1 second HIGH, 0.5 second LOW

bool actEdge = false, actTimer;

int pinLed = 19;

void ledControl()
{
  static bool state = false;
  state = !state;
  digitalWrite(pinLed, state);
}

void ledOn()
{
  digitalWrite(pinLed, true);
}

void ledOff()
{
  digitalWrite(pinLed, false);
}


void setup()
{
  pinMode(pinLed, OUTPUT);
  Serial.begin(115200);
  //timer1.setCallback(ledControl);
  timerPWM.setCallback(ledOn,ledOff);
}

void loop()
{
  bot1.update();
  bool dRead = bot1.getDigitalRead();
  timer1.update(!dRead);
  timerPWM.update(true);

  bool actPWM = timerPWM.getTimerOutput();
  static bool cmdLed = bot1.getDigitalRead();

  //*FLANCO*/

  // Serial.println((String)"Toggle: "+actEdge+" Dread= "+dRead);
  Serial.println(actPWM);
 // Serial.println(timerPWM.getElapsedTime());
  //digitalWrite(pinLed,actPWM);
}





//***Include, define y espacios de nombre
#include <Arduino.h>
#include <Timer.h>
using namespace GlobalLibrary;


//***Declaración de funciones


//***Declaración de variables y objetos
int pinP1 = 16, pinP2 = 17;
int pinLed=19;

Timer timerLed(200,1000);

void setup()
{
  pinMode(pinP1, INPUT_PULLUP);
  pinMode(pinP2, INPUT_PULLUP);
  pinMode(pinLed, OUTPUT);
  Serial.begin(115200);
}

void loop()
{
  bool stsPinP1 = (digitalRead(pinP1)==0) ? false : true;
  bool stsPinP2 = !(bool)digitalRead(pinP2);
  if()
}

//** Implementación de funciones */

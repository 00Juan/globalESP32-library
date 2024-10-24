/*
En este ejemplo se muestra un ejemplo de implementación de la clase Timer, en el que se hará
parpadear un led y se ejecutarán varias rutinas temporizadas sin hacer uso de la función delay()
*/

//***Include, define y espacios de nombre
#include <Arduino.h>
#include <Timer.h>
using namespace GlobalLibrary;

//** Declaración de funciones */

//** Declaración de variables y objetos */
int pinLed = 19;

// Configura el timer en modo PWM para que la salida esté activa durante 200ms e inactiva durante 1000ms
Timer timerLed(200, 1000);

void setup()
{
    pinMode(pinLed, OUTPUT);
    Serial.begin(115200);
}

void loop()
{


    
}

//** Implementación de funciones */

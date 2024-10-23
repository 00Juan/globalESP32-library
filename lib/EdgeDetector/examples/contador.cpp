/*En este ejemplo se implementa un contador ascendente/descendente configurable con cuatro pulsadores en configuración pull-up:
p1Cont -> Cada vez que se pulse o se suelte, incrementa o decrementa el valor de la cuenta según el valor de las variables "contSum" y "contUpDown"
p2UpDown -> Cada vez que se pulse, el contador cambiará a ser ascendente si era descendente y viceversa
p3SumDecrease -> Cada vez que se suelte, disminuirá en una unidad el valor de "contSum"
p4SumIncrease -> Cada vez que se pulse, aumentará en una unidad el valor de "contSum"

Se hará uso de un led que se encenderá sólo cuando p1Cont esté pulsado y haya actuado el antirebotes
*/

//***Include, define y espacios de nombre
#include <Arduino.h>
#include <EdgeDetector.h>
using namespace GlobalLibrary;
#define DEBOUNCE_TIME 20 // Tiempo del antirebotes en ms

//***Declaración de funciones

//***Declaración de variables y objetos

int pinP1 = 16, pinP2 = 17, pinP3 = 18, pinP4 = 21;
int pinLed = 19;
int contSum = 1;        // Cantidad a incrementar/decrementar
int contValue = 0;      // Valor del contador
bool contUpDown = true; // false -> down, true -> up

// Al estar las entradas digitales en configuración pull-up, cada vez que se pulse alguno de los pulsadores el valor de la entrada
// pasará de 1 a 0 por lo que se detectará un flanco de bajada

EdgeDetector p1Cont(pinP1, EDGE_TOGGLE, PIN_INPUT_PULLUP, DEBOUNCE_TIME);
EdgeDetector p2UpDown(pinP2, EDGE_FALLING, PIN_INPUT_PULLUP, DEBOUNCE_TIME);
EdgeDetector p3SumDecrease(pinP3, EDGE_RISING, PIN_INPUT_PULLUP, DEBOUNCE_TIME);
EdgeDetector p4SumIncrease(pinP4, EDGE_FALLING, PIN_INPUT_PULLUP, DEBOUNCE_TIME);

void contSumDecrease()
{
  contSum--;
  if (contSum < 0)
    contSum = 0; // impide que baje de 0
  Serial.println((String) "Actualización suma a: " + contSum);
}

void setup()
{
  pinMode(pinLed, OUTPUT);
  Serial.begin(115200);

  // Se configura el objeto p3sumDecrease para que ejecute la función contSumDecrease() cada
  // vez que se detecte un flanco de subida. El segundo argumento es para ejecutar otra función
  // en el flanco de bajada, en este caso se pone "nullptr (puntero que apunta a nada)" ya que
  // no se va a ejecutar ninguna función
  // IMPORTANTE -> La función a ejecutar no debe de tener argumentos y ha de retornar void
  p3SumDecrease.setCallback(contSumDecrease, nullptr);
}

void loop()
{
  //**Actualizamos los objetos para que estén atentos a las posibles pulsaciones
  p1Cont.update();
  p2UpDown.update();
  p3SumDecrease.update();
  p4SumIncrease.update();

  //**Gestión del p1
  if (p1Cont.getEdgeDetected())
  {
    if (contUpDown)
      contValue += contSum; // Equivale a contValue=contValue+contSum
    else
      contValue -= contSum;

    Serial.println((String) "Actualización contador a: " + contValue);
  }

  //**Gestión del p2
  if (p2UpDown.getEdgeDetected())
  {
    contUpDown = !contUpDown; // Invierte el valor actual de la variable (OJO!!, no es "!=" ;  "!" por si solo niega y "!=" compara)
    if (contUpDown)
      Serial.println("Contador modo UP");
    else
      Serial.println("Contador modo DOWN");
  }

  //**Gestión del p3 mediante callback en el setup()

  // Gestión del p4
  if (p4SumIncrease.getEdgeDetected())
  {
    contSum++;
    Serial.println((String) "Actualización suma a: " + contSum);
  }
  
  //**Gestión del led

  // getDigitalRead() a nivel de uso es identico a digitalRead(), sólo que deja actuar el antirrebotes
  bool cmdActLed = !p1Cont.getDigitalRead();
  digitalWrite(pinLed, cmdActLed);
}

#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

HX711 balanca;
float calibration_factor = 39130;

void setup() {
  Serial.begin(9600);
  balanca.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  Serial.println();
  Serial.println("HX711 - Calibracao da Balança");
  balanca.set_scale();
  balanca.tare();
}

void zeraBalanca()
{
  Serial.println(); // salta uma linha
  balanca.tare(); // zera a Balança
  Serial.println("Balança Zerada ");
}

void loop() {

  Serial.print("Peso: ");   // imprime no monitor serial
  Serial.print(balanca.get_units(), 3);   // imprime peso na balança com 3 casas decimais 
  Serial.println(" kg");   // imprime no monitor serial 
  delay(500) ;   // atraso de 0,5 segundos 
  if (Serial.available())   // se a serial estiver disponivel
  {
   char temp = Serial.read();   // le carcter da serial 
   if (temp == 't' || temp == 'T')   // se pressionar t ou T
   {
   balanca.tare();   // zera a balança
   Serial.println(" Balança zerada");   // imprime no monitor serial
   }
  }
}

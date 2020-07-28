/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */

// -- Pin change interrupt
#include <PciManager.h>
#include <PciListenerImp.h>
//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio



#define INPUT_PIN 3

void onPinChange(byte changeKind);
PciListenerImp listener(INPUT_PIN, onPinChange);


void setup() {
  Serial.begin(9800);
  PciManager.registerListener(&listener);
  Serial.println("Ready.");
}

void onPinChange(byte changeKind) {
  // -- changeKind can be: CHANGEKIND_HIGH_TO_LOW, or CHANGEKIND_LOW_TO_HIGH HIGH
  Serial.print("pci : ");
  Serial.println(changeKind);
}

void loop() { }

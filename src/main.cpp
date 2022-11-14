#include <Arduino.h>
#include "TouchControl.h"

TouchControl touchControl;

void setup() {
  Serial.begin(115200);
  
  touchControl.begin(T0);
  touchControl.addToDictionary('S', 100, 1000, true);
  touchControl.addToDictionary('L', 1000, 5000, true);
  touchControl.addToDictionary('P', 100, 1000, false);
  touchControl.registerPattern("SPSPL");
}

void loop() {
  bool patternFound = touchControl.checkPattern();

  if (patternFound){
    Serial.printf("Successful pattern %s \n", touchControl.getLastPattern());
    //Serial.println("unlocked");
  }
}
#include <SPI.h>
#include <SDLogger.h>
#include <WebServer.h>
#include "TouchControl.h"

SDLogger sdLog;
WebServer server;
TouchControl touchControl;

long currentTime;
String loopMessage = "";

void setup()
{
  Serial.begin(115200);
  delay(2000);
  Serial.println("");
  Serial.println("Lock start");
  touchControl.begin(T0);
  touchControl.addToDictionary('S', 100, 1000, true);
  touchControl.addToDictionary('L', 1000, 5000, true);
  touchControl.addToDictionary('P', 100, 1000, false);
  touchControl.registerPattern("SPSPL");

  if (!sdLog.Begin("/log.txt"))
  {
    return;
  }

  server.Begin("smartlock", "/WebManager", "TheLabIOT", "Yaay!ICanTalkNow", &sdLog);
  server.Start();

  sdLog.WriteLine("Setup complete");
}
void loop()
{
	bool patternFound = touchControl.checkPattern();

  if (patternFound){
    Serial.printf("Successful pattern %s \n", touchControl.getLastPattern());
    //Serial.println("unlocked");
  }
  
  currentTime = millis();
  loopMessage = String(currentTime);
  loopMessage.concat(" loop");

  sdLog.WriteLine(loopMessage, false);
  delay(10000);
}

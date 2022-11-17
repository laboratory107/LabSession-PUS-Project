#include <SPI.h>
#include <SDLogger.h>
#include <WebServer.h>
#include <TouchControl.h>

SDLogger sdLog;
WebServer server;
TouchControl touchControl;

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

  if (!sdLog.begin("/log.txt"))
  {
    return;
  }

  server.begin("smartlock", "/WebManager", "LAB5-IOT","noKotu2i0u", &sdLog);
  server.start();

  sdLog.writeLine("Setup complete");
}
void loop()
{
	if (touchControl.checkPattern()){
    Serial.printf("Successful pattern %s \n", touchControl.getLastPattern());
    sdLog.writeLine("Pattern found - unlocked");
  }
}

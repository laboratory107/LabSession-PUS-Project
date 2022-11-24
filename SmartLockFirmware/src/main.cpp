#include <SPI.h>
#include <SDLogger.h>
#include <WebServer.h>
#include <oled.h>
#include <TouchControl.h>

SDLogger sdLog;
WebServer server;
TouchControl touchControl;

OLED *oled;

void setup()
{
  Serial.begin(115200);
  oled = new OLED(128,64,0x3C); 
  delay(2000);
  Serial.println("");
  Serial.println("Lock start");
  oled->drawMessage("Lock start");
  delay(1000);

  touchControl.begin(T0);
  touchControl.addToDictionary('S', 100, 1000, true);
  touchControl.addToDictionary('L', 1000, 5000, true);
  touchControl.addToDictionary('P', 100, 1000, false);
  touchControl.registerPattern("SPSPL");

  if (!sdLog.begin("/log.txt"))
  {
    oled->drawError("No SD log.");
    delay(1000);
    return;
  }

  server.begin("smartlock", "/WebManager", "LAB5-IOT","noKotu2i0u", &sdLog);
  server.start();

  sdLog.writeLine("Setup complete");
  oled->drawMessage("Setup complete.");
  delay(1000);
}
void loop()
{
	if (touchControl.checkPattern())
  {
    Serial.printf("Successful pattern %s \n", touchControl.getLastPattern());
    sdLog.writeLine("Pattern found - unlocked");
    oled->drawAccess(true);
    delay(5000);
  }
  else
  {
    oled->drawAccess(false);
    delay(1000);
  }
}

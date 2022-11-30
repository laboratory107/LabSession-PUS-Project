#include <SPI.h>
#include <SDLogger.h>
#include <WebServer.h>
#include <oled.h>
#include <TouchControl.h>

#define SSID "AC Huawei"
#define PASSWORD "xyz"

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
  Serial.printf("Lock start. T0 (touch) pin is %d\n",T0);
  oled->drawMessage("Lock start");
  delay(1000);

  touchControl.begin(T0);
  touchControl.addToDictionary('S', 100, 500, true);
  touchControl.addToDictionary('L', 501, 2000, true);
  touchControl.addToDictionary('P', 100, 2000, false);
  touchControl.registerPattern("SPSPL");

  if (!sdLog.begin("/log.txt"))
  {
    oled->drawError("No SD log.");
    delay(1000);
    return;
  }

  //server.begin("smartlock", "/WebManager", "LAB5-IOT","noKotu2i0u", &sdLog);
  server.begin("smartlock", "/WebManager", SSID,PASSWORD, &sdLog);
  server.start();

  sdLog.writeLine("Setup complete");
  oled->drawMessage("Setup complete.");
  delay(1000);
  oled->drawAccess(false);
}
void loop()
{
	if (touchControl.checkPattern())
  {
    Serial.printf("Successful pattern %s \n", touchControl.getLastPattern());
    sdLog.writeLine("Pattern found - unlocked");
    oled->drawAccess(true);
    delay(5000);
    oled->drawAccess(false);
  }
}

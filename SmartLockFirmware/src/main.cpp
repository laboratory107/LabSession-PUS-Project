#include <SPI.h>
#include <SDLogger.h>
#include <WebServer.h>

long currentTime;
String loopMessage = "";
SDLogger sdLog;
WebServer server;

void setup()
{
  Serial.begin(9600);
  delay(2000);
  Serial.println("");
  Serial.println("Lock start");

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
  currentTime = millis();
  loopMessage = String(currentTime);
  loopMessage.concat(" loop");

  sdLog.WriteLine(loopMessage, false);
  delay(10000);
}

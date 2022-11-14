#include <SPI.h>
#include <SDLogger.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>

AsyncWebServer server(80);

const char *ssid = "TheLabIOT";
const char *password = "Yaay!ICanTalkNow";

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

long currentTime;
String message = "";

SDLogger sdLog;

void StartServer()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    WiFi.begin(ssid, password);
    Serial.printf("WiFi Failed!\n");
    delay(1000);
  }
  MDNS.begin("smartlock");
  // TODO: http:// mora biti

  // TODO: metode za wifi setup i SD begin u Setup.
  //

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // enumerate html files
  File folder = SD.open("/WebManager", FILE_READ);
  folder.rewindDirectory();
  String on = "";
  while (true)
  {
    File entry = folder.openNextFile();
    if (!entry)
    {
      // no more files
      break;
    }

    sdLog.WriteLine("new file " + String(entry.name()));
    on = "/";
    on.concat(entry.name());
    on = on.substring(on.lastIndexOf("/"));

    // https://wokwi.com/projects/new/esp32

    sdLog.WriteLine("file registration: " + on);

    server.on(on.c_str(), HTTP_GET, [&](AsyncWebServerRequest *request)
              {
                sdLog.WriteLine("new request "+request->url());
                String content = SD.open("/WebManager"+ request->url() ).readString();

                if(request->url().equals("/log.html"))
                {
                  if(request->hasArg("clear"))
                  {
                    sdLog.WriteLine("log clear requested", false);
                    sdLog.Clear();
                  }
                  String logContent = sdLog.ReadAll();
                  content.replace("{1}", logContent);
                }
                request->send(200, "text/html",  content ); });
    entry.close();
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              sdLog.WriteLine("new request ");
              request->send(200, "text/html", SD.open("/WebManager/index.html").readString()); });

  /*
    server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request)
              {
          String message;
          if (request->hasParam(PARAM_MESSAGE, true)) {
              message = request->getParam(PARAM_MESSAGE, true)->value();
          } else {
              message = "No message sent";
          }
          request->send(200, "text/plain", "Hello, POST: " + message); });
  */
  server.onNotFound(notFound);
  server.begin();
  sdLog.WriteLine("Server setup done");
}
void setup()
{
  Serial.begin(9600);
  delay(2000);
  Serial.println("");
  Serial.println("");
  sdLog.Begin("/log.txt");

  StartServer();
  sdLog.WriteLine("Setup complete");
}
void loop()
{
  currentTime = millis();
  message = String(currentTime);
  message.concat(" loop");

  sdLog.WriteLine(message, false);
  delay(10000);
}

#include <SPI.h>
#include <SD.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <mdns.h>

AsyncWebServer server(80);

const char *ssid = "TheLabIOT";
const char *password = "Yaay!ICanTalkNow";

const char *PARAM_MESSAGE = "message";

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

File file;
long currentTime;
String message = "";

void WriteToLog(String logEntry)
{
  file = SD.open("/log.txt", FILE_WRITE);
  file.seek(file.size(), SeekMode::SeekEnd);
  file.println(logEntry);
  file.close();
  Serial.println("writing to log: " + logEntry);
}

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
  mdns_init();
  mdns_hostname_set("smartlock");
  // mdns_service_add(NULL, "_http", "_tcp", 80, NULL, 0);
  mdns_instance_name_set("Best lock ever!");

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
    WriteToLog("new file " + String(entry.name()));
    on = "";
    on.concat(entry.name());
    on = on.substring(on.lastIndexOf("/"));
    // on = on.substring(0, on.indexOf("."));
    // https://wokwi.com/projects/new/esp32

    WriteToLog("file registration: " + on);

    server.on(on.c_str(), HTTP_GET, [&](AsyncWebServerRequest *request)
            {
                WriteToLog("new request "+request->url());
                String content = SD.open("/WebManager"+ request->url() ).readString();

                if(request->url().equals("/log.html"))
                {
                  String log = SD.open("/log.txt").readString();
                  content.replace("{1}", log);
                }
                request->send(200, "text/html",  content ); 
            });
    entry.close();
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              WriteToLog("new request ");
              request->send(200, "text/html", SD.open("/WebManager/index.html").readString()); });

  server.on("/log.html", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              WriteToLog("new request 3");
              request->send(200, "text/html", SD.open("/WebManager/log.html").readString()); });

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
  WriteToLog("Server setup done");
}
void setup()
{
  Serial.begin(9600);
  delay(2000);

  if (!SD.begin())
  {
    Serial.println("begin failed");
    return;
  }

  StartServer();
  WriteToLog("Setup complete");
}
void loop()
{

  /*
    currentTime = millis();
    message = String(currentTime);
    message.concat(" loop");
    WriteToLog(message);

    delay(3000);*/
}
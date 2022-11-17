#include "WebServer.h"

WebServer::WebServer()
{
    server = new AsyncWebServer(80);
}

bool WebServer::begin(String ServerName, String RootPath, String WiFiSSID, String WiFiPassword, SDLogger *logger)
{
    bool returnMe = false;
    sdLog = logger;
    this->serverName = ServerName;
    this->path = RootPath;

    WiFi.mode(WIFI_STA);
    WiFi.begin(WiFiSSID.c_str(), WiFiPassword.c_str());
    sdLog->writeLine("Connecting to WiFi...");
    while (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        sdLog->writeLine(".", false);
        delay(600);
    }
    sdLog->writeLine("Wifi connected, IP Address: " + WiFi.localIP().toString());

    WiFi.setHostname("smartlock");
    if (!MDNS.begin("smartlock"))
    {
        sdLog->writeLine("MDNS error");
        while (1)
        {
            delay(1000);
        }
        // TODO: http:// mora biti
    }
    sdLog->writeLine("MDNS name: " + serverName);

    // enumerate html files
    File folder = SD.open(this->path, FILE_READ);
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

        sdLog->writeLine("new file " + String(entry.name()));
        on = "/";
        on.concat(entry.name());
        on = on.substring(on.lastIndexOf("/"));
        // https://wokwi.com/projects/new/esp32

        sdLog->writeLine("file registration: " + on);

        server->on(on.c_str(), HTTP_GET, [&](AsyncWebServerRequest *request)
                   {
                sdLog->writeLine("new request "+request->url());
                String content = SD.open("/WebManager"+ request->url() ).readString();

                if(request->url().equals("/log.html"))
                {
                  if(request->hasArg("clear"))
                  {
                    sdLog->writeLine("log clear requested", false);
                    sdLog->clear();
                  }
                  String logContent = "id=\"log\">";
                  logContent.concat(sdLog->readAll());
                  content.replace("id=\"log\">", logContent);
                }
                if(request->url().equals("/manager.html"))
                {
                  File fileKeys = SD.open("/keys.txt", FILE_READ);
                  String keyContent = "id=\"keys\">";
                  keyContent.concat( fileKeys.readString());
                  fileKeys.close();
                  content.replace("id=\"keys\">", keyContent);
                }
                if(request->url().equals("/save.html"))
                {
                    String saveMe ="";
                    for (size_t i = 0; i < request->params(); i++)
                    {
                        saveMe.concat(request->getParam(i)->name());
                        saveMe.concat("=");
                        saveMe.concat(request->getParam(i)->value());
                        saveMe.concat("\n");
                    }
                File fileKeys = SD.open("/keys.txt", FILE_WRITE);
                fileKeys.print(saveMe);
                fileKeys.close();
                }
                request->send(200, "text/html",  content ); });
        entry.close();
    }//while enumerate files

    server->on("/", HTTP_GET, [&](AsyncWebServerRequest *request)
               {
                sdLog->writeLine("new request ");
                request->send(200, "text/html", SD.open("/WebManager/index.html").readString()); });

    server->onNotFound([&](AsyncWebServerRequest *request)
                       {
                    request->send(404, "text/plain", "Not found");
                    sdLog->writeLine("404 request "); });

    sdLog->writeLine("Server setup done");
    return returnMe;
}
bool WebServer::start()
{
    bool returnMe = false;
    server->begin();
    sdLog->writeLine("Server started");
    return returnMe;
}
bool WebServer::stop()
{
    bool returnMe = false;
    server->end();
    sdLog->writeLine("Server stopped");
    return returnMe;
}
WebServer::~WebServer()
{
}
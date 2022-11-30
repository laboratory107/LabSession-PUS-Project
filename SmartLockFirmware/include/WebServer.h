#include <SD.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>

#include <SDLogger.h>

class WebServer
{
private:
    String serverName;
    String path;
    AsyncWebServer *server;
    SDLogger *sdLog;
public:

    WebServer();
    /// @brief
    /// @param ServerName  including path, root is /
    bool begin(String ServerName, String RootPath, String WiFiSSID, String WiFiPassword, SDLogger *logger);
    bool start();
    bool stop();
    ~WebServer();
};

#include "SDLogger.h"

bool SDLogger::Begin(String FileName)
{
    isReady = false;
    if (!SD.begin())
    {
        Serial.println("begin failed");
        return isReady;
    }
    this->fileName = FileName;
    isReady = true;
    return isReady;
}
void SDLogger::WriteLine(String logEntry, bool saveToCard)
{
    Serial.println("Log: " + logEntry);
    if (saveToCard && isReady)
    {
        logEntry.concat("\n");
        logEntry.concat(ReadAll());

        fileWrite = SD.open(this->fileName, FILE_WRITE);
        fileWrite.print(logEntry);
        fileWrite.close();
    }
}
String SDLogger::ReadAll()
{
    String returnMe = "";
    if(!isReady)
    {
        return returnMe;
    }
    
    fileRead = SD.open(this->fileName, FILE_READ);
    returnMe.concat(fileRead.readString());
    fileRead.close();
    return returnMe;
}
void SDLogger::Clear()
{
    if(!isReady)
    {
        return;
    }
    
    fileWrite = SD.open(this->fileName, FILE_WRITE);
    fileWrite.println("");
    fileWrite.close();
}
SDLogger::SDLogger()
{
}
SDLogger::~SDLogger()
{
    fileWrite.close();
    fileRead.close();
}

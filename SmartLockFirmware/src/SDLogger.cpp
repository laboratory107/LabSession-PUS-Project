#include "SDLogger.h"

void SDLogger::Begin(String FileName)
{
    if (!SD.begin())
    {
        Serial.println("begin failed");
        return;
    }
    this->fileName = FileName;
   // fileWrite = SD.open(this->fileName, FILE_WRITE);
    //fileRead = SD.open(this->fileName, FILE_READ);
    //Serial.println("log file: " + String(fileWrite.name()));
}
void SDLogger::WriteLine(String logEntry, bool saveToCard)
{
    Serial.println("Log: " + logEntry);
    if (saveToCard)
    {
        //String saveMe = "";
        //saveMe.concat(logEntry);
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
    fileRead = SD.open(this->fileName, FILE_READ);
    returnMe.concat(fileRead.readString());
    fileRead.close();
    return returnMe;
}
void SDLogger::Clear()
{
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

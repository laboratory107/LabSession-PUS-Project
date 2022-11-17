#pragma once
#include <SD.h>

class SDLogger
{
private:
    String fileName;
    File fileWrite;
    File fileRead;
    bool isReady;
public:

    SDLogger();
    /// @brief
    /// @param FileName  including path, root is /
    bool begin(String FileName);
    ~SDLogger();
    void writeLine(String logEntry, bool saveToCard = true);
    String readAll();
    void clear();
};
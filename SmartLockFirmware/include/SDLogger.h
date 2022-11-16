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
    void WriteLine(String logEntry, bool saveToCard = true);
    String ReadAll();
    void Clear();
};
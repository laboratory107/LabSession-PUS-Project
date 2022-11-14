#include <SD.h>

class SDLogger
{
private:
    String fileName;
    File fileWrite;
    File fileRead;
public:
    /// @brief 
    /// @param FileName  including path, root is /
    SDLogger();
    void Begin(String FileName);
    ~SDLogger();
    void WriteLine(String logEntry, bool saveToCard = true);
    String ReadAll();
    void Clear();
};
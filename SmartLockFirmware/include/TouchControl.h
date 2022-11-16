#include <Arduino.h>
#include <map>
#include <vector>

struct patternItem
{
    int min;
    int max;
    char label;
    bool state;

    patternItem() {}
    patternItem(char l, int mi, int ma, bool st) : min(mi), max(ma), label(l), state(st)
    {
    }
};

class TouchControl
{
private:
    int touchPin = T0;

    const uint8_t threshold = 20;

    const long touchDelay = 200;

    volatile unsigned long currentTime, lastRegisteredTouchTime, anyTouchTime = 0;
    volatile unsigned long firstPauseTime, firstTouchTime = 0;
    volatile unsigned long pauseLength, touchLength = 0;

    long elapsed = 0;
    int touchCounter = 0;

    String lastTouchPattern = "";
    String lastCommand = "";

    static void IRAM_ATTR touchInterrupt();
    char getTouchPattern(int);

    Stream *debugStream;
    bool debugEnabled = false;

    std::vector<patternItem> patternDictionary;
    std::vector<String> patterns;

    bool getPatternForLabel(char label, patternItem &item);
    bool getPatternForDuration(int duration, bool state, patternItem &item);
    bool checkCurrentPatternForDuration(int, bool);
    bool checkIsPatternValid(String);
    bool checkIsPartialPatternValid(String);

public:
    void begin(int pin = T0, Stream &stream = Serial);
    void enableDebug(Stream &stream = Serial);
    void setupTouchInterrupt();
    bool checkPattern();
    String getLastPattern();
    void addToDictionary(char, int, int, bool);
    void registerPattern(String);
};
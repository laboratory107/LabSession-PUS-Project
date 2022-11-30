#ifndef OLED_H
#define OLED_H
#include <Wire.h>
#include <cstring>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


class OLED
{
    public:
        OLED(int width, int heigh, int address);
        void drawError(const String& text);
        void drawMessage(const String& text);
        void drawPattern(const String& hint, const String& pattern);
        void drawAccess(bool granted);
    private:
        Adafruit_SSD1306 *screen;
        bool available;
        int width;
        int height;
        void getTextSize(const String& text, unsigned short &w, unsigned short &h);

};

#endif
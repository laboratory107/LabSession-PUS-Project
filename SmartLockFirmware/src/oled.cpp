#include "oled.h"

OLED::OLED(int width, int height, int address)
{
    this->width = width;
    this->height = height;
    available = true;
    screen = new Adafruit_SSD1306(width, height, &Wire, -1);
    if(screen->begin(SSD1306_SWITCHCAPVCC, address))
    {
        delay(1000);
        screen->setTextColor(WHITE);
        screen->clearDisplay();
        screen->display();
        available = true;
    }
}

void OLED::drawMessage(const String& text)
{
    int i;
    unsigned short w, h;
    if(!available)
        return;
    screen->clearDisplay();
    for(i=0; i<6; i+=2)
        screen->drawRect(i,i,width-2*i,height-2*i,WHITE);
    screen->setTextSize(1);
    getTextSize(text, w, h);
    screen->setCursor((width-w)/2,(height-h)/2);
    screen->print(text);
    screen->display();
}

void OLED::drawPattern(const String& hint, const String& pattern)
{
    unsigned short w, h;
    if(!available)
        return;
    screen->clearDisplay();
    screen->setTextSize(0);
    screen->setCursor(0,0);
    screen->print(hint);
    screen->setTextSize(2);
    getTextSize(pattern, w, h);
    screen->setCursor((width-w)/2,(height-h)/2);
    screen->print(pattern);

    screen->display();
}

void OLED::drawError(const String& text)
{
    unsigned short w, h, off;
    String err = "E R R O R";
    if(!available)
        return;
    screen->clearDisplay();
    screen->drawRect(0,0,width,height,WHITE);
    screen->drawRect(2,2,width-4,height-4,WHITE);
    screen->setTextSize(2);
    getTextSize(err, w, h);
    screen->setCursor((width-w)/2,(height-h)/2);
    off=(height-h)/2+h+4;
    screen->print(err.c_str());
    screen->setTextSize(0);
    getTextSize(text,w,h);
    screen->setCursor((width-w)/2,off);
    screen->println(text.c_str());
    screen->display();
}

void OLED::drawAccess(bool granted)
{
    unsigned short w, h;
    String gr[2] = {"ACCESS","GRANTED"};
    String dn[2] = {"CHECK","PATTERN"};
    String *ptr;
    if(!available)
        return;
    if(granted)
        ptr=&gr[0];
    else
        ptr=&dn[0];
    screen->clearDisplay();
    screen->setTextSize(2);
    screen->drawRect(0,0,width,height,WHITE);
    getTextSize(*ptr,w,h);
    screen->setCursor((width-w)/2,(height-2*h)/2);
    screen->print(*ptr);
    ptr++;
    getTextSize(*ptr,w,h);
    screen->setCursor((width-w)/2,(height+h)/2);
    screen->print(*ptr);
    screen->display();
}

void OLED::getTextSize(const String& text, unsigned short &w, unsigned short &h)
{
    short x1, y1;
    if(!available)
        return;
    screen->getTextBounds(text,0,0,&x1, &y1, &w, &h);
}

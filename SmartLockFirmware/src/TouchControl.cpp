#include "TouchControl.h"
#include "oled.h"

extern OLED *oled;

bool touched = false;
void IRAM_ATTR TouchControl::touchInterrupt()
{
    touched = true;
}

void TouchControl::begin(int pin, Stream &stream)
{
    touchPin = pin;
    debugStream = &stream;
    debugEnabled = true;

    setupTouchInterrupt();
}

bool TouchControl::checkPattern()
{
    currentTime = millis();

    if (touched)
    {
        touched = false;
        elapsed = currentTime - lastRegisteredTouchTime;
        if (elapsed > touchDelay)
        {
            lastRegisteredTouchTime = currentTime;

            if (firstTouchTime == 0)
                firstTouchTime = currentTime;

            if (firstPauseTime > 0)
            {
                pauseLength = currentTime - firstPauseTime;
                debugStream->printf("Pause length: %d \n", pauseLength);
                firstPauseTime = 0;

                if (checkCurrentPatternForDuration(pauseLength, false))
                    return true;
            }

            touchCounter++;
        }

        anyTouchTime = currentTime;
    }
    else
    {
        if (currentTime - anyTouchTime > touchDelay && touchCounter > 0) // If there hasn't been a touch for a good moment
        {
            // lastTouchPattern = lastTouchPattern + getTouchPattern(touchCounter);
            touchLength = currentTime - firstTouchTime;
            debugStream->printf("Touch length: %d \n", touchLength);
            firstTouchTime = 0;
            firstPauseTime = currentTime;

            if (checkCurrentPatternForDuration(touchLength, true))
                return true;

            touchCounter = 0;
        }
    }

    return false;
}

void TouchControl::enableDebug(Stream &stream)
{
    debugStream = &stream;
    debugEnabled = true;
}

void TouchControl::setupTouchInterrupt()
{
    touchAttachInterrupt(touchPin, touchInterrupt, threshold);
}

void TouchControl::addToDictionary(char label, int min, int max, bool state)
{
    patternItem item = patternItem(label, min, max, state);
    if (getPatternForLabel(label, item))
    {
        if (debugEnabled)
            debugStream->printf("Pattern with label %c already exists", label);

        return;
    }

    patternDictionary.push_back(item);
}

bool TouchControl::getPatternForLabel(char label, patternItem &item)
{
    auto existingPattern = std::find_if(patternDictionary.begin(), patternDictionary.end(), [&label](const patternItem &pattern)
                                        { return pattern.label == label; });

    if (existingPattern != patternDictionary.end())
    {
        item = *existingPattern;
        return true;
    }

    return false;
}

bool TouchControl::getPatternForDuration(int duration, bool state, patternItem &item)
{
    auto existingPattern = std::find_if(patternDictionary.begin(), patternDictionary.end(), [&duration, &state](const patternItem &pattern)
                                        { return pattern.min <= duration && pattern.max >= duration && pattern.state == state; });

    if (existingPattern != patternDictionary.end())
    {
        item = *existingPattern;
        return true;
    }

    return false;
}

bool TouchControl::checkCurrentPatternForDuration(int duration, bool state)
{
    patternItem pattern;
    if (getPatternForDuration(duration, state, pattern)) // If we have a recognized a valid touch, add it to the current pattern
    {
        lastTouchPattern += pattern.label;
        debugStream->printf("Current pattern: %s \n", lastTouchPattern);
        oled->drawPattern("Current pattern:",lastTouchPattern);
    }

    if (!checkIsPartialPatternValid(lastTouchPattern)) // If the current partial pattern is not a part of any patterns in the list, reset the current pattern
    {
        if (debugEnabled)
            debugStream->printf("Touch pattern interrupted, last pattern: %s \n", lastTouchPattern);
        oled->drawPattern("Int - last pattern:",lastTouchPattern);
        if(lastTouchPattern.length()>8)
        {
            oled->drawAccess(false);
            delay(1000);
            lastTouchPattern = "";
            lastRegisteredTouchTime = currentTime;
        }
        return false;
    }

    return checkIsPatternValid(lastTouchPattern);
}

bool TouchControl::checkIsPatternValid(String pattern)
{
    auto existingPattern = std::find(patterns.begin(), patterns.end(), pattern);

    if (existingPattern != patterns.end())
    {
        if (debugEnabled)
            debugStream->printf("Touch pattern found: %s \n", lastTouchPattern);

        return true;
    }

    return false;
}

bool TouchControl::checkIsPartialPatternValid(String partialPattern)
{
    auto existingPattern = std::find_if(patterns.begin(), patterns.end(), [&partialPattern](const String &pattern)
                                        { return pattern.startsWith(partialPattern); });
                                        
    return existingPattern != patterns.end();
}

String TouchControl::getLastPattern()
{
    String temp = lastTouchPattern;
    lastTouchPattern = "";
    return lastTouchPattern;
}

void TouchControl::registerPattern(String pattern)
{
    patternItem item;

    for (char label : pattern)
    {
        if (!getPatternForLabel(label, item))
        {
            if (debugEnabled)
                debugStream->printf("Pattern with label %c does not exist", label);

            return;
        }
    }

    patterns.push_back(pattern);
}
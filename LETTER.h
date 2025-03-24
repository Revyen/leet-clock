// LETTER.h
#ifndef LETTER_h
#define LETTER_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "RGB.h"

class LETTER {
  private:
    int startPixelNumber;
    int endPixelNumber;
    bool on;
    Adafruit_NeoPixel ledStripRef;
    RGB color;

  public:
    LETTER(int startPixel, int endPixel, Adafruit_NeoPixel& ledStrip);
    LETTER(int startPixel, int endPixel, Adafruit_NeoPixel& ledStrip, RGB initialColor);
    void turnOn();
    void turnOn(RGB rgb);
    void turnOff();
    bool getState();
};

#endif
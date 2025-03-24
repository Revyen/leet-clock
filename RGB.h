// RGB.h
#ifndef RGB_h
#define RGB_h

#include <Arduino.h>

class RGB {
  private:
    int _red;
    int _green;
    int _blue;

  public:
    RGB();
    RGB(int red, int green, int blue);
    int red();
    int green();
    int blue();
    String toString();
    RGB deepRed();
    RGB rose();
    RGB magenta();
    RGB violet();
    RGB deepBlue();
    RGB azure();
    RGB cyan();
    RGB springGreen();
    RGB deepGreen();
    RGB chartreuse();
    RGB yellow();
    RGB orange();
    RGB random12Color();
    RGB random6Color();
    RGB random3Color();
};

#endif
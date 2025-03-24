// RGB.cpp
#include "RGB.h"

RGB::RGB() {
  _red = 0;
  _green = 0;
  _blue = 0;
}

RGB::RGB(int r, int g, int b) {
  _red = r % 256;
  _green = g % 256;
  _blue = b % 256;
}

int RGB::red() {
  return _red;
}

int RGB::green() {
  return _green;
}

int RGB::blue() {
  return _blue;
}

String RGB::toString() {
  return "R: " + String(_red) + ", G: " + String(_green) + ", B: " + String(_blue);
}

RGB RGB::deepRed() {
  return RGB(255,0,0);
}
RGB RGB::rose() {
  return RGB(255,0,128);
}
RGB RGB::magenta() {
  return RGB(255,0,255);
}
RGB RGB::violet() {
  return RGB(128,0,255);
}
RGB RGB::deepBlue() {
  return RGB(0,0,255);
}
RGB RGB::azure() {
  return RGB(0,128,255);
}
RGB RGB::cyan() {
  return RGB(0,255,255);
}
RGB RGB::springGreen() {
  return RGB(0,255,128);
}
RGB RGB::deepGreen() {
  return RGB(0,255,0);
}
RGB RGB::chartreuse() {
  return RGB(128,255,0);
}
RGB RGB::yellow() {
  return RGB(255,255,0);
}
RGB RGB::orange() {
  return RGB(255,128,0);
}

RGB RGB::random12Color() {
  int rand = random(0,12);
  switch(rand) {
    case 0:
      return deepRed();
    case 1:
      return rose();
    case 2:
      return magenta();
    case 3:
      return violet();
    case 4:
      return deepBlue();
    case 5:
      return azure();
    case 6:
      return cyan();
    case 7:
      return springGreen();
    case 8:
      return deepGreen();
    case 9:
      return chartreuse();
    case 10:
      return yellow();
    case 11:
      return orange();
  }
  return RGB(255,255,255);
}

RGB RGB::random6Color() {
  int rand = random(0,6);
  switch(rand) {
    case 0:
      return deepRed();
    case 1:
      return magenta();
    case 2:
      return deepBlue();
    case 3:
      return cyan();
    case 4:
      return deepGreen();
    case 5:
      return yellow();
  }
  return RGB(255,255,255);
}

RGB RGB::random3Color() {
  int rand = random(0,3);
  switch(rand) {
    case 0:
      return deepRed();
    case 1:
      return deepBlue();
    case 2:
      return deepGreen();
  }
  return RGB(255,255,255);
}
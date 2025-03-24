#include "LETTER.h"

// Konstruktor ohne initiale Farbe
LETTER::LETTER(int startPixel, int endPixel, Adafruit_NeoPixel& ledStrip)
  : startPixelNumber(startPixel), endPixelNumber(endPixel), ledStripRef(ledStrip), on(false), color(RGB(0, 0, 0)) {}

// Konstruktor mit initialer Farbe
LETTER::LETTER(int startPixel, int endPixel, Adafruit_NeoPixel& ledStrip, RGB initialColor)
  : startPixelNumber(startPixel), endPixelNumber(endPixel), ledStripRef(ledStrip), on(false), color(initialColor) {}

// Methode zum Einschalten der LEDs mit der aktuellen Farbe
void LETTER::turnOn() {
  for (int i = startPixelNumber; i <= endPixelNumber; i++) {
    ledStripRef.setPixelColor(i, ledStripRef.Color(color.red(), color.green(), color.blue()));
  }
  ledStripRef.show();
  on = true;
}

// Methode zum Einschalten der LEDs mit einer neuen Farbe
void LETTER::turnOn(RGB rgb) {
  color = rgb;
  turnOn();
}

// Methode zum Ausschalten der LEDs
void LETTER::turnOff() {
  for (int i = startPixelNumber; i <= endPixelNumber; i++) {
    ledStripRef.setPixelColor(i, ledStripRef.Color(0, 0, 0));
  }
  ledStripRef.show();
  on = false;
}

// Methode zum Abfragen des aktuellen Zustands (ein/aus)
bool LETTER::getState() {
  return on;
}

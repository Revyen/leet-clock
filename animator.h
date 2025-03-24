// animator.h
#ifndef ANIMATOR_H
#define ANIMATOR_H
#include "LETTER.h"
#include "RGB.h"
#include "animation.h"

class Animator {
  private:
    Adafruit_NeoPixel& ledStripRef;
    LETTER* letters;
    int numLetters;
    int numPixels;
    int animationStep;
    int phaseStep;
    bool playing;
    bool repeat;
    int repeatTimes;
    int repeatCount;
    int randomLedOrder[41];
    Animation animation;
    bool animate(RGB color, int width, bool reverse);
    bool leftToRight(RGB color, int width);
    bool rightToLeft(RGB color, int width);
    bool pingPong(RGB color, int width);
    bool fade(RGB color, int width, int stepDiff, bool reverse);
    bool fadeIn(RGB color, int width, int stepDiff);
    bool fadeOut(RGB color, int width, int stepDiff);
    bool pulse(RGB color, int width, int stepDiff);
    bool hello(RGB color);
    bool sparkle(int changesPerTick, int stepDiff, int numTimes);
    void shuffleLedOrder();

  public:
    Animator(Adafruit_NeoPixel& ledStrip, int numPixels, LETTER* letters, int numLetters);
    void startAnimation(const Animation& animation);
    void startAnimation(const Animation& animation, bool repeatAnimation);
    void startAnimation(const Animation& animation, bool repeatAnimation, int repeatTimes);
    bool tick();
    bool getPlaying();
    void reset();
};

#endif

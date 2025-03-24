// animation.h
#ifndef ANIMATION_H
#define ANIMATION_H
#include "RGB.h"

class Animation {
  public:
    enum AnimationName {
      LEFT_TO_RIGHT,
      RIGHT_TO_LEFT,
      PING_PONG,
      FADE_IN,
      FADE_OUT,
      PULSE,
      HELLO,
      NONE,
      SPARKLE
    };
  private:
    AnimationName name;
    RGB color;
    int width;
  public:
    Animation();
    Animation(AnimationName name, RGB color, int width);
    AnimationName getName();
    RGB getColor();
    int getWidth();
};

#endif
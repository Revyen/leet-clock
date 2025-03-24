// animation.cpp
#include <Arduino.h>
#include "animation.h"

// Default constructor is needed
Animation::Animation() : name(NONE), color(0, 0, 0), width(0) {}

Animation::Animation(AnimationName name, RGB color, int width)
  : name(name), color(color), width(width) {}

Animation::AnimationName Animation::getName() {
  return name;
}

RGB Animation::getColor() {
  return color;
}

int Animation::getWidth() {
  return width;
}
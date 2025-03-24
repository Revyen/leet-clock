// animator.cpp
#include <Arduino.h>
#include "animator.h"
#include <cstring>

Animator::Animator(Adafruit_NeoPixel& ledStrip,int numPixels, LETTER* letters, int numLetters)
  : ledStripRef(ledStrip), letters(letters), numLetters(numLetters), numPixels(numPixels) {
    animationStep = -1;
    phaseStep = -1;
    playing = false;
    repeat = false;
    shuffleLedOrder();
  }

void Animator::shuffleLedOrder() {
  for (int i = 0; i < numPixels; i++) {
    randomLedOrder[i] = i;
  }

  randomSeed(analogRead(0));
  
  for (int i = 0; i < numPixels; ++i) {
      int j = random(0, numPixels);
      int temp = randomLedOrder[i];
      randomLedOrder[i] = randomLedOrder[j];
      randomLedOrder[j] = temp;
  }
}

bool Animator::getPlaying() {
  return playing;
}

void Animator::startAnimation(const Animation& newAnimation) {
  reset();
  Serial.println("Starting Animation...");
  animation = newAnimation;
  Serial.println(animation.getName() + " started");
  playing = true;
  Serial.println(playing);
}

void Animator::startAnimation(const Animation& newAnimation, bool repeatAnimation) {
  startAnimation(newAnimation);
  repeat = repeatAnimation;
}

void Animator::startAnimation(const Animation& newAnimation, bool repeatAnimation, int repeatTimesAnimation) {
  startAnimation(newAnimation, repeatAnimation);
  repeatTimes = repeatTimesAnimation;
  repeatCount = 0;
}

bool Animator::tick() {
  if (!playing) {
    return true;
  }
  animationStep++;

  bool finished = false;

  switch(animation.getName()) {
    case Animation::LEFT_TO_RIGHT:
      finished = leftToRight(animation.getColor(), animation.getWidth());
      break;
    case Animation::RIGHT_TO_LEFT:
      finished = rightToLeft(animation.getColor(), animation.getWidth());
      break;
    case Animation::PING_PONG:
      finished = pingPong(animation.getColor(), animation.getWidth());
      break;
    case Animation::FADE_IN:
      finished = fadeIn(animation.getColor(), animation.getWidth(), 0);
      break;
    case Animation::FADE_OUT:
      finished = fadeOut(animation.getColor(), animation.getWidth(), 0);
      break;
    case Animation::PULSE:
      finished = pulse(animation.getColor(), animation.getWidth(), 0);
      break;
    case Animation::HELLO:
      finished = hello(animation.getColor());
      break;
    case Animation::SPARKLE:
      finished = sparkle(2,0, animation.getWidth());
      break;
    case Animation::NONE:
      break;
  }

  if (finished) {
    Serial.println(repeat);
    Serial.println(repeatCount);
    Serial.println(repeatTimes);
    Serial.println(repeatCount < repeatTimes);
    if (repeat && repeatTimes == 0 || repeatCount < repeatTimes) {
      animationStep = 0;
      phaseStep = 0;
      shuffleLedOrder();
      for (int i = 0; i < numPixels; i++) {
        ledStripRef.setPixelColor(i, 0,0,0);
      }
      if (repeatTimes != 0) {
        repeatCount++;
      }
      return false;
    }
    reset();
  }

  return finished;
}

void Animator::reset() {
  playing = false;
  repeat = false;
  animationStep = 0;
  phaseStep = 0;
  repeatCount = 0;
  repeatTimes = 0;
  for (int i = 0; i < numPixels; i++) {
    ledStripRef.setPixelColor(i, 0,0,0);
  }
}

bool Animator::sparkle(int changesPerTick, int stepDiff, int numTimes) {
  bool finished = false;
  int finishingStep = numPixels * changesPerTick * numTimes;

  for (int i = changesPerTick * animationStep; i < changesPerTick * (animationStep + 1); i++) {
    int ledToChange = randomLedOrder[i % numPixels];
    RGB randomRGB = RGB().random6Color();
    ledStripRef.setPixelColor(ledToChange, ledStripRef.Color(randomRGB.red(), randomRGB.green(), randomRGB.blue()));
  }

  if ((animationStep % numPixels) == 0) {
    shuffleLedOrder();
  }

  if (animationStep == finishingStep) {
    finished = true;
  }

  return finished;
}

bool Animator::hello(RGB color) {
  bool finished = false;

  switch(animationStep) {
    case 10:
      letters[0].turnOn(color);
      break;
    case 20:
      letters[1].turnOn(color);
      break;
    case 30:
      letters[2].turnOn(color);
      break;
    case 40:
      letters[3].turnOn(color);
      break;
    case 50:
      letters[4].turnOn(color);
      break;
  }

  if (animationStep >= 60 && animationStep <= 85) {
    fadeOut(color, 25, 60);
  }

  if (animationStep >= 85) {
    finished = pulse(color, 50, 85);
  }

  return finished;
}

bool Animator::fade(RGB color, int width, int stepDiff, bool reverse) {
  bool finished = false;
  int step = animationStep - stepDiff;

  float f_width = static_cast<float>(width);
  float f_animationStep = static_cast<float>(step); 
  float stepFrac = f_animationStep / f_width;
  float f_red = static_cast<float>(color.red()) * stepFrac;
  float f_green = static_cast<float>(color.green()) * stepFrac;
  float f_blue = static_cast<float>(color.blue()) * stepFrac;
  if (reverse) {
    f_red = color.red() - f_red;
    f_green = color.green() - f_green;
    f_blue = color.blue() - f_blue;
  }
  for (int i = 0; i < numPixels; i++) {
    ledStripRef.setPixelColor(i, 
      ledStripRef.Color(
        static_cast<int>(f_red),
        static_cast<int>(f_green),
        static_cast<int>(f_blue)
      )
    );
  }

  if (step == width + 1) {
    finished = true;
  }

  return finished;
}

bool Animator::fadeIn(RGB color, int width, int stepDiff) {
  return fade(color, width, stepDiff, false);
}

bool Animator::fadeOut(RGB color, int width, int stepDiff) {
  return fade(color, width, stepDiff, true);
}

bool Animator::pulse(RGB color, int width, int stepDiff) {
  bool finished = false;
  int step = animationStep - stepDiff;

  if (step <= width / 2) {
    fadeIn(color, width, stepDiff);
  } else {
    fadeOut(color, width, stepDiff);
  }

  if (step == width + 1) {
    finished = true;
  }

  return finished;
}

bool Animator::animate(RGB color, int width, bool reverse) {
  bool finished = false;
  int currentStep = reverse ? (numPixels - 1 - animationStep) : animationStep;
  int endStep = (numPixels - 1) + width;

  if (animationStep < numPixels) {
    ledStripRef.setPixelColor(currentStep, ledStripRef.Color(color.red(), color.green(), color.blue()));
  }

  int widthEnd = reverse ? (numPixels - 1 - animationStep + width) : (currentStep - width);
  if (widthEnd >= 0 && widthEnd < numPixels) {
    ledStripRef.setPixelColor(widthEnd, 0, 0, 0);
  }

  if (animationStep == endStep) {
    finished = true;
  }

  return finished;
}

bool Animator::leftToRight(RGB color, int width) {
  return animate(color, width, false);
}

bool Animator::rightToLeft(RGB color, int width) {
  return animate(color, width, true);
}

bool Animator::pingPong(RGB color, int width) {
  bool finished = false;

  switch (phaseStep) {
    case 0:
      leftToRight(color, width);
      break;
    case 1:
      rightToLeft(color, width);
      break;
    case 2:
      leftToRight(color, width);
      break;
    default:
      finished = true;
      break;
  }

  if (animationStep == (numPixels-1) + width) {
    phaseStep++;
    animationStep = 0;
  }

  return finished;
}


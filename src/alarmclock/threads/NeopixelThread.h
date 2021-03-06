#pragma once
#include <Arduino.h>

#include "CThread.h"
#include <types.h>

#include <Adafruit_NeoPixel.h>

struct NeopixelCommand {
  u32 color;
  u32 brightness;
};

struct neopixelThread {
  CThread* (*initialize)(u8 priority);
};

extern const struct neopixelThread NeopixelThread;
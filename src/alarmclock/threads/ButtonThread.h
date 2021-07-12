#pragma once

#include "CThread.h"
#include <Arduino.h>
#include <types.h>
#include "bithelper.h"

#define KEY_EVENTBITMASK 0x00FFFFFFU
#define KEY_LIGHT 0x0001
#define KEY_ONOFF 0x0002
#define KEY_REVIEW 0x0004
#define KEY_DISP 0x0008
#define KEY_SET 0x0010
#define KEY_HOURUP 0x0020
#define KEY_HOURDOWN 0x0040
#define KEY_MINUP 0x0080
#define KEY_MINDOWN 0x0100
#define KEY_TEACH 0x0200
#define KEY_ZZZ 0x0400
#define KEY_OFF 0x0800
// unused
// #define KEY12 0x1000
// #define KEY13 0x2000
// #define KEY14 0x4000
#define KEYLONG 0x8000

struct buttonThread {
  CThread* (*initialize)(u8 priority);
};

extern const struct buttonThread ButtonThread;
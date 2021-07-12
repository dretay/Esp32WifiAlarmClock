#include "NeopixelThread.h"

using namespace std;

#define NEOPIXEL_QUEUE_SIZE 10
#define LED_PIN 32
#define LED_COUNT 12
// #define DEFAULTBRIGHTNESS 50
// QueueHandle_t NEOPIXEL_QUEUE;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

static CThread thread;
u32 stripBrightness = 0, stripColor = 0;
enum NEOPIXELSTATUS { OFF, ON } neoPixelStatus;

static void colorWipe() {
  // if (neoPixelStatus == ON) {
  strip.setBrightness(stripBrightness);
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, stripColor);
    strip.show();
    delay(100);
  }
  // }
}
static void colorImmediate() {
  // if (neoPixelStatus == ON) {
  strip.setBrightness(stripBrightness);
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, stripColor);
  }
  strip.show();
  // }
}

static void run(void* params) {
  struct NeopixelCommand command;  // = {.command = NeopixelCommand::NOOP, .value = 0};

  while (true) {
    xQueueReceive(thread.cmdMsgQueue, &command, portMAX_DELAY);
    if (uxQueueMessagesWaiting(thread.cmdMsgQueue) > 0) {
      xQueueReceive(thread.cmdMsgQueue, &command, portMAX_DELAY);
    }
    Serial.printf_P(PSTR("Processing neopixel message\n"));
    stripColor = command.color;
    stripBrightness = command.brightness;
    colorWipe();
    // switch (command.command) {
    //   case NeopixelCommand::COLOR:
    //     stripColor = command.value;
    //     colorWipe();
    //     break;

    //   case NeopixelCommand::STATUS:
    //     neoPixelStatus = (NEOPIXELSTATUS)command.value;
    //     colorImmediate();
    //     break;

    //   case NeopixelCommand::BRIGHTNESS:
    //     stripBrightness = command.value;
    //     colorImmediate();

    //   case NeopixelCommand::NOOP:
    //     break;
    // }
    // command.command = NeopixelCommand::NOOP;
  }
  vTaskDelete(NULL);
}

static CThread* initialize(u8 priority) {
  strip.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  thread.run = run;
  // NEOPIXEL_QUEUE = xQueueCreate(10, sizeof(NeopixelCommand));
  // if (NEOPIXEL_QUEUE == NULL) {
  //   Serial.printf_P(PSTR("Error creating the queue"));
  // }
  return CThread_super(&thread, 2048, "neopixelThread", (tskIDLE_PRIORITY + priority), sizeof(NeopixelCommand));
}
const struct neopixelThread NeopixelThread = {
    .initialize = initialize,
};
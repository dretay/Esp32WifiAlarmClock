#include "NeopixelThread.h"

using namespace std;

#define NEOPIXEL_QUEUE_SIZE 10
#define LED_PIN 32
#define LED_COUNT 12
// #define DEFAULTBRIGHTNESS 50
QueueHandle_t NEOPIXEL_QUEUE;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

static CThread thread;

static void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) {  // For each pixel in strip...
    strip.setPixelColor(i, color);               //  Set pixel's color (in RAM)
    strip.show();                                //  Update strip to match
    delay(wait);                                 //  Pause for a moment
  }
}

static void run(void* params) {
  bool first = true;
  struct NeopixelCommand command = {.command = NeopixelCommand::NOOP, .value = 0};
  strip.show();
  strip.setBrightness(5);  // TODO: this should be pulled from _state in contructor
  while (true) {
    xQueueReceive(NEOPIXEL_QUEUE, &command, 0);
    switch (command.command) {
      case NeopixelCommand::COLOR:
        colorWipe(command.value, 50);
        break;
      case NeopixelCommand::ENABLED:

        break;

      case NeopixelCommand::BRIGHTNESS:
        strip.setBrightness(command.value);
        strip.show();
        command.command = NeopixelCommand::NOOP;
    }
    if (first) {
      colorWipe(strip.Color(255, 0, 0), 50);  // Red
      first = false;
    }
    // colorWipe(strip.Color(0, 255, 0), 50);     // Green
    // colorWipe(strip.Color(0, 0, 255), 50);     // Blue
    // colorWipe(strip.Color(0, 0, 0, 255), 50);  // True white (not RGB white)
  }
  vTaskDelete(NULL);
}

static CThread* initialize(u8 priority) {
  strip.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  thread.run = run;
  NEOPIXEL_QUEUE = xQueueCreate(10, sizeof(NeopixelCommand));
  if (NEOPIXEL_QUEUE == NULL) {
    Serial.printf_P(PSTR("Error creating the queue"));
  }
  return CThread_super(&thread, 2048, "neopixelThread", (tskIDLE_PRIORITY + priority));
}
const struct neopixelThread NeopixelThread = {
    .initialize = initialize,
};
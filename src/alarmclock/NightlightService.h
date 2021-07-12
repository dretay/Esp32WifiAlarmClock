#ifndef NightlightService_h
#define NightlightService_h

#include <HttpEndpoint.h>
#include <WebSocketTxRx.h>
#include <FSPersistence.h>
#include "../types.h"
#include "threads/NeopixelThread.h"

#define NIGHTLIGHT_SETTINGS_ENDPOINT_PATH "/rest/nightlightState"
#define NIGHTLIGHT_SETTINGS_SOCKET_PATH "/ws/nightlightState"
#define NIGHTLIGHT_SETTINGS_FILE "/config/nightlightSettings.json"

struct Time {
  int hours;
  int minutes;
};

class NightlightState {
 public:
  u32 status;
  u32 color;
  u32 brightness;  // 0-255
  time_t start;
  time_t stop;

  static void read(NightlightState& settings, JsonObject& root) {
    root["status"] = settings.status;
    root["color"] = settings.color;
    root["brightness"] = settings.brightness;
    root["start"] = settings.start;
    root["stop"] = settings.stop;
  }
  static StateUpdateResult update(JsonObject& root, NightlightState& nightlightState) {
    // extern QueueHandle_t NEOPIXEL_QUEUE;
    int newStatus = root["status"];
    int newColor = root["color"];
    int newBrightness = root["brightness"];
    time_t newStart = root["start"];
    time_t newStop = root["stop"];
    boolean stateChanged = false;
    // Serial.printf_P(PSTR("Neopixel update: color - %d brightness - %d\n"), newColor, newBrightness);

    if (nightlightState.status != newStatus) {
      stateChanged = true;
      nightlightState.status = root["status"];
      if (nightlightState.status == 2) {  // todo: if in "test" then should always be on, otherwise "off" or noop?
        // struct NeopixelCommand command = {
        //     .command = NeopixelCommand::STATUS,
        //     .value = 1,
        // };
        // xQueueSend(NEOPIXEL_QUEUE, &command, portMAX_DELAY);
      }
    }
    if (nightlightState.color != newColor) {
      stateChanged = true;
      nightlightState.color = root["color"];
      // Serial.printf_P(PSTR("Neopixel color changed\n"));
      // struct NeopixelCommand command = {
      //     .command = NeopixelCommand::COLOR,
      //     .value = newColor,
      // };
      // xQueueSend(NEOPIXEL_QUEUE, &command, portMAX_DELAY);
    }
    if (nightlightState.brightness != newBrightness) {
      stateChanged = true;
      nightlightState.brightness = root["brightness"];
      // Serial.printf_P(PSTR("Neopixel brightness changed\n"));

      // struct NeopixelCommand command = {
      //     .command = NeopixelCommand::BRIGHTNESS,
      //     .value = newBrightness,
      // };
      // xQueueSend(NEOPIXEL_QUEUE, &command, portMAX_DELAY);
    }
    if (nightlightState.start != newStart) {
      stateChanged = true;
      nightlightState.start = root["start"];
    }
    if (nightlightState.stop != newStop) {
      stateChanged = true;
      nightlightState.stop = root["stop"];
    }
    if (stateChanged) {
      // Serial.printf_P(PSTR("Neopixel state changed\n"));
      return StateUpdateResult::CHANGED;
    }
    // Serial.printf_P(PSTR("Neopixel state unchanged\n"));
    return StateUpdateResult::UNCHANGED;
  }
};

class NightlightService : public StatefulService<NightlightState> {
 public:
  NightlightService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);
  void begin();
  void loop();

 private:
  HttpEndpoint<NightlightState> _httpEndpoint;
  WebSocketTxRx<NightlightState> _webSocket;
  FSPersistence<NightlightState> _fsPersistence;

  void registerConfig();
  void onConfigUpdated();
};

#endif
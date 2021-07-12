#include "NightlightService.h"

static CThread* neopixelThread;
NightlightService::NightlightService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _httpEndpoint(NightlightState::read,
                  NightlightState::update,
                  this,
                  server,
                  NIGHTLIGHT_SETTINGS_ENDPOINT_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _webSocket(NightlightState::read,
               NightlightState::update,
               this,
               server,
               NIGHTLIGHT_SETTINGS_SOCKET_PATH,
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED),
    _fsPersistence(NightlightState::read, NightlightState::update, this, fs, NIGHTLIGHT_SETTINGS_FILE) {
  // configure settings service update handler to update LED state
  addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}
static bool isNightlightOn = false;
static struct Time convertTime(time_t storedTimestamp) {
  int storedHours = 0;
  int storedMinutes = 0;
  if (storedTimestamp > 1000) {
    char storedTsHours[3];
    char storedTsMinutes[3];
    // Serial.printf_P(PSTR("storedTimestamp %d \n"), storedTimestamp);
    struct tm storedTs = *gmtime(&storedTimestamp);
    strftime(storedTsHours, 3, "%H", &storedTs);
    strftime(storedTsMinutes, 3, "%M", &storedTs);
    storedHours = atoi(storedTsHours);
    storedMinutes = atoi(storedTsMinutes);
  }
  return Time{
      .hours = storedHours,
      .minutes = storedMinutes,
  };
}
void NightlightService::loop() {
  delay(1000);
  // static bool isNightlightOn = _state.brightness;

  if (_state.status == 1) {
    struct Time startTime = convertTime(_state.start);
    struct Time stopTime = convertTime(_state.stop);
    struct Time currTime = convertTime(time(nullptr));

    bool shouldBeOn = false;
    if (currTime.hours >= startTime.hours && currTime.hours < stopTime.hours) {
      shouldBeOn = true;
    } else if (currTime.minutes >= startTime.minutes && currTime.minutes < stopTime.minutes) {
      shouldBeOn = true;
    }
    if (shouldBeOn && !isNightlightOn) {
      isNightlightOn = true;
      struct NeopixelCommand command = {
          .color = _state.color,
          .brightness = _state.brightness,
      };
      xQueueSend(neopixelThread->cmdMsgQueue, &command, portMAX_DELAY);
    } else if (!shouldBeOn && isNightlightOn) {
      isNightlightOn = false;
      struct NeopixelCommand command = {
          .color = _state.color,
          .brightness = 0,
      };
      xQueueSend(neopixelThread->cmdMsgQueue, &command, portMAX_DELAY);
    }
  }
}
void NightlightService::begin() {
  Serial.printf_P(PSTR("Starting Nightlight Service...\n"));
  neopixelThread = NeopixelThread.initialize(3);
  _fsPersistence.readFromFS();
  Serial.printf_P(PSTR("Nightlight Service Started!\n"));
}

void NightlightService::onConfigUpdated() {
  if (_state.status == 0) {
    struct NeopixelCommand command = {
        .color = _state.color,
        .brightness = 0,
    };
    xQueueSend(neopixelThread->cmdMsgQueue, &command, portMAX_DELAY);
  } else if (_state.status == 2) {
    struct NeopixelCommand command = {
        .color = _state.color,
        .brightness = _state.brightness,
    };
    xQueueSend(neopixelThread->cmdMsgQueue, &command, portMAX_DELAY);
  }
}

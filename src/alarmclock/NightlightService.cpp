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
  // config goes here
}
void NightlightService::loop() {
}
void NightlightService::begin() {
  Serial.printf_P(PSTR("Starting Nightlight Service...\n"));
  neopixelThread = NeopixelThread.initialize(3);
  _fsPersistence.readFromFS();
  Serial.printf_P(PSTR("Nightlight Service Started!\n"));
}

void NightlightService::onConfigUpdated() {
  // config update broadcast goes here
}

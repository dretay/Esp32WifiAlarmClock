#include <ESP8266React.h>
// #include <LightMqttSettingsService.h>
// #include <LightStateService.h>
#include <alarmclock/AlarmService.h>
#include <alarmclock/NightlightService.h>
#include <FS.h>

#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);
ESP8266React esp8266React(&server);
// LightMqttSettingsService lightMqttSettingsService =
//     LightMqttSettingsService(&server, esp8266React.getFS(), esp8266React.getSecurityManager());
// LightStateService lightStateService = LightStateService(&server,
//                                                         esp8266React.getSecurityManager(),
//                                                         esp8266React.getMqttClient(),
//                                                         &lightMqttSettingsService);
NightlightService nightlightService = NightlightService(&server, &SPIFFS, esp8266React.getSecurityManager());
AlarmService alarmService = AlarmService(&server, &SPIFFS, esp8266React.getSecurityManager());
HardwareSerial dfplayerUart(1);

void setup() {
  // start serial and filesystem
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.printf_P(PSTR("Application starting up...\n"));

  // start the file system (must be done before starting the framework)
#ifdef ESP32
  SPIFFS.begin(true);
#elif defined(ESP8266)
  SPIFFS.begin();
#endif

  // start the framework and demo project
  esp8266React.begin();

  // load the initial light settings
  // lightStateService.begin();

  // load the initial alarm settings
  alarmService.begin();

  nightlightService.begin();

  // start the light service
  // lightMqttSettingsService.begin();

  // start the server
  server.begin();

  Serial.printf_P(PSTR("Application started!\n"));
}

void loop() {
  // run the framework's loop function
  esp8266React.loop();
  nightlightService.loop();
  // alarmService.loop();
}

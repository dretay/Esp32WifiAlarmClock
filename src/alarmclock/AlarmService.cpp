#include "AlarmService.h"

extern ESP8266React esp8266React;

AlarmService::AlarmService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _httpEndpoint(AlarmSettings::read,
                  AlarmSettings::update,
                  this,
                  server,
                  ALARM_SETTINGS_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _fsPersistence(AlarmSettings::read, AlarmSettings::update, this, fs, ALARM_SETTINGS_FILE) {
  alarmTriggered = false;
}

// extern QueueHandle_t VOICE_QUEUE;
// extern QueueHandle_t MP3_QUEUE;
extern QueueHandle_t keypadQueue;
extern EventGroupHandle_t keypadEventGroup;

static CThread *voiceThread, *mp3Thread, *buttonThread, *displayThread;

static void toggleVoiceThread(void* param) {
  while (true) {
    // speak, makes sense to wait forever
    xEventGroupWaitBits(keypadEventGroup, KEY_OFF, pdTRUE, pdTRUE, portMAX_DELAY);

    int command = 0;
    // xQueueSend(VOICE_QUEUE, &command, portMAX_DELAY);
    xQueueSend(voiceThread->cmdMsgQueue, &command, portMAX_DELAY);
  }
}
static int mp3_command = 0;
static void toggleMp3Thread(void* param) {
  while (true) {
    xEventGroupWaitBits(keypadEventGroup, KEY_REVIEW, pdTRUE, pdTRUE, portMAX_DELAY);
    // xQueueSend(MP3_QUEUE, &mp3_command, portMAX_DELAY);
    xQueueSend(mp3Thread->cmdMsgQueue, &mp3_command, portMAX_DELAY);

    mp3_command++;
  }
}
static void memoryWatchdog(void* param) {
  while (true) {
    vTaskDelay(pdMS_TO_TICKS(5000));
    Serial.println("=============Memory Report=============");
    voiceThread->memoryFree(voiceThread);
    mp3Thread->memoryFree(mp3Thread);
    // neopixelThread->memoryFree(neopixelThread);
    buttonThread->memoryFree(buttonThread);
    displayThread->memoryFree(displayThread);
    Serial.println("=======================================");
  }
}
static void syncSystemState(void* param) {
  while (true) {
    esp8266React.getWiFiSettingsService()->read(
        [&](WiFiSettings& wifiSettings) { SystemState.set_ssid(wifiSettings.ssid.c_str()); });

    SystemState.set_sntp_sync_status(sntp_get_sync_status());
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
static void gotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  SystemState.set_ip(WiFi.localIP().toString().c_str());
}
void AlarmService::begin() {
  Serial.printf_P(PSTR("Starting Alarm Service...\n"));
  _fsPersistence.readFromFS();

  voiceThread = VoiceThread.initialize(3);
  mp3Thread = Mp3Thread.initialize(0);

  buttonThread = ButtonThread.initialize(0);
  displayThread = DisplayThread.initialize(0);
  WiFi.onEvent(gotIP, WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);

  xTaskCreate(toggleVoiceThread, "toggleVoiceThread", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1), NULL);
  xTaskCreate(toggleMp3Thread, "toggleMp3Thread", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1), NULL);
  xTaskCreate(syncSystemState, "syncSystemState", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1), NULL);
  // xTaskCreate(memoryWatchdog, "memoryWatchdog", 2048, NULL, (tskIDLE_PRIORITY + 1), NULL);
  Serial.printf_P(PSTR("Alarm Service Started!\n"));
}

void AlarmService::loop() {
  // delay(1000);
  // xEventGroupWaitBits(keypadEventGroup, TOGGLE_LED_Green_KEY, pdTRUE, pdTRUE, 0);
  // Serial.printf_P(PSTR("Key 0 Pressed"));

  // char storedTsHours[3];
  // char storedTsMinutes[3];
  // time_t storedTimestamp = _state.entries[0].time;
  // struct tm storedTs = *gmtime(&storedTimestamp);
  // strftime(storedTsHours, 3, "%H", &storedTs);
  // strftime(storedTsMinutes, 3, "%M", &storedTs);
  // int storedHours = atoi(storedTsHours);
  // int storedMinutes = atoi(storedTsMinutes);

  // char nowTsHours[3];
  // char nowTsMinutes[3];
  // time_t currentTimestamp = time(nullptr);
  // struct tm nowTs = *gmtime(&currentTimestamp);
  // strftime(nowTsHours, 3, "%H", &nowTs);
  // strftime(nowTsMinutes, 3, "%M", &nowTs);
  // int nowHours = atoi(nowTsHours);
  // int nowMinutes = atoi(nowTsMinutes);

  // if (storedHours == nowHours && storedMinutes == nowMinutes) {
  //   if (alarmTriggered == false) {
  //     alarmTriggered = true;
  //     int speak = 0;
  //     int play = 1;
  //     // xQueueSend(queue, &speak, portMAX_DELAY);
  //     // xQueueSend(queue, &play, portMAX_DELAY);
  //   } else {
  //     // this is 2
  //   }
  // } else {
  //   // digitalWrite(S0, LOW);
  //   // digitalWrite(S1, HIGH);
  //   alarmTriggered = false;
  // }
  // whiteOverRainbow(75, 5);
  // // neopixels->whiteOverRainbow(75, 5);

  // if (interruptCounter > 0) {
  //   portENTER_CRITICAL(&mux);
  //   interruptCounter--;
  //   portEXIT_CRITICAL(&mux);

  //   numberOfInterrupts++;
  //   if (alarmTriggered) {
  //     // myMP3.stop();
  //   }
  // }
}
#include "DisplayThread.h"

static CThread thread;

static View *wifiStatusView, *clockView;

static void run(void* params) {
  Serial.printf_P(PSTR("ClockThread running...\n"));
  bool have_shown_ip = false;

  // Infinite loop to run main bulk of task
  while (1) {
    vTaskDelay(pdMS_TO_TICKS(1000));
    if (SystemState.get_sntp_sync_status() == SNTP_SYNC_STATUS_IN_PROGRESS || have_shown_ip == false) {
      wifiStatusView->render();
    } else {
      clockView->render();
    }
    if (SystemState.ip[0] != '\0' && have_shown_ip == false) {
      have_shown_ip = true;
      wifiStatusView->render();
      vTaskDelay(pdMS_TO_TICKS(2000));
    }
  }

  // Out side of loop now. Task needs to clean up and self terminate before returning
  vTaskDelete(NULL);
}

static CThread* initialize(u8 priority) {
  gfxInit();
  wifiStatusView = WifiStatusView.init();
  clockView = ClockView.init();
  thread.run = run;
  return CThread_super(&thread, 2048, "displayThread", (tskIDLE_PRIORITY + priority), 0);
}
const struct displayThread DisplayThread = {
    .initialize = initialize,
};

#pragma once 

#include <WiFi.h>
#include "CThread.h"
#include <Arduino.h>
#include <types.h>
#include <ugfx-config-uc1698.h>
#include <ESP8266React.h>
#include "../views/WifiStatusView.h"
#include "../views/ClockView.h"

struct displayThread
{
	CThread*(*initialize)(u8 priority);	
};

extern const struct displayThread DisplayThread;
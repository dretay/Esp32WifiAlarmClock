#pragma once

#define MAX_SSID_LENGTH 32
#define MAX_IP_LENGTH 16
#include "sntp.h"

struct systemstate
{
    char *ssid;
    char *ip;
    sntp_sync_status_t(*get_sntp_sync_status)(void);
    void(*init)(void);
    void(*set_sntp_sync_status)(sntp_sync_status_t sntp_sync_status_in);
    void(*set_ssid)(const char* ssid_in);
    void(*set_ip)(const char* ip_in);
    void(*reset)(void);
};

extern volatile const struct systemstate SystemState;
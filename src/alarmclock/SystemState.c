#include "SystemState.h"

static char ssid[MAX_SSID_LENGTH] = {'\0'};
static char ip[MAX_IP_LENGTH] = {'\0'};
static sntp_sync_status_t sntp_sync_status;

static void set_ssid(const char* ssid_in){
    strncpy(ssid,ssid_in,MAX_SSID_LENGTH);
}
static void set_ip(const char* ip_in){
    strncpy(ip,ip_in,MAX_IP_LENGTH);
}
static void set_sntp_sync_status(sntp_sync_status_t sntp_sync_status_in){
    sntp_sync_status = sntp_sync_status_in;
}
static sntp_sync_status_t get_sntp_sync_status(void){
    return sntp_sync_status;
}
static void reset(void)
{
    void;
}


static init(){
}

volatile const struct systemstate SystemState = { 	
	.init = init, 
    .ssid = ssid,
    .ip = ip,
    .set_sntp_sync_status = set_sntp_sync_status,
    .get_sntp_sync_status = get_sntp_sync_status,
    .set_ssid = set_ssid,
    .set_ip = set_ip,
    .reset = reset,	
};
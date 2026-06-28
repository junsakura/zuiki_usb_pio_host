#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    bool usb_connected;
    bool hid_connected;

    uint16_t vid;
    uint16_t pid;

    uint16_t report_len;

    uint8_t report[64];

} usb_status_t;

extern usb_status_t g_usb_status;

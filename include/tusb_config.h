#pragma once

// Pico SDK sets CFG_TUSB_MCU from PICO_BOARD=pico2.
// Do NOT define CFG_TUSB_MCU here.

// TinyUSB OS
#define CFG_TUSB_OS OPT_OS_PICO

// USB-C native port: Device CDC
#define CFG_TUSB_RHPORT0_MODE (OPT_MODE_DEVICE | OPT_MODE_FULL_SPEED)

// USB-A port through Pico-PIO-USB: Host HID
#define CFG_TUSB_RHPORT1_MODE (OPT_MODE_HOST | OPT_MODE_FULL_SPEED)
#define CFG_TUH_RPI_PIO_USB 1
#define BOARD_TUH_RHPORT 1

// Device: USB CDC logger on Type-C
#define CFG_TUD_ENABLED 1
#define CFG_TUD_ENDPOINT0_SIZE 64
#define CFG_TUD_CDC 1
#define CFG_TUD_CDC_RX_BUFSIZE 256
#define CFG_TUD_CDC_TX_BUFSIZE 256
#define CFG_TUD_CDC_EP_BUFSIZE 64
#define CFG_TUD_MSC 0
#define CFG_TUD_HID 0
#define CFG_TUD_MIDI 0
#define CFG_TUD_VENDOR 0

// Host: HID only
#define CFG_TUH_ENABLED 1
#define CFG_TUH_DEVICE_MAX 4
#define CFG_TUH_ENDPOINT_MAX 8
#define CFG_TUH_ENUMERATION_BUFSIZE 256
#define CFG_TUH_HUB 1
#define CFG_TUH_HID 4
#define CFG_TUH_CDC 0
#define CFG_TUH_MSC 0
#define CFG_TUH_VENDOR 0

#define CFG_TUSB_MEM_SECTION
#define CFG_TUSB_MEM_ALIGN __attribute__((aligned(4)))

#pragma once

#include <stdint.h>

void hid_dispatch_mount(
    uint8_t dev_addr,
    uint8_t instance,
    uint16_t vid,
    uint16_t pid);

void hid_dispatch_unmount(
    uint8_t dev_addr,
    uint8_t instance);

void hid_dispatch_report(
    uint8_t dev_addr,
    uint8_t instance,
    const uint8_t *report,
    uint16_t len);

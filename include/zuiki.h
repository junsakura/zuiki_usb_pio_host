#pragma once

#include <stdint.h>

void zuiki_on_hid_mounted(
    uint8_t dev_addr,
    uint8_t instance,
    uint16_t vid,
    uint16_t pid);

void zuiki_on_hid_unmounted(
    uint8_t dev_addr,
    uint8_t instance);

void zuiki_on_hid_report(
    uint8_t dev_addr,
    uint8_t instance,
    const uint8_t *report,
    uint16_t len);

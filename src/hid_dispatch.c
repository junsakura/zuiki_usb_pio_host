#include "hid_dispatch.h"
#include "logger.h"
#include "zuiki.h"

void hid_dispatch_mount(
    uint8_t dev_addr,
    uint8_t instance,
    uint16_t vid,
    uint16_t pid)
{
    log_printf(
        "Dispatch VID=%04X PID=%04X\r\n",
        vid,
        pid);

    zuiki_on_hid_mounted(
        dev_addr,
        instance,
        vid,
        pid);
}

void hid_dispatch_unmount(
    uint8_t dev_addr,
    uint8_t instance)
{
    zuiki_on_hid_unmounted(
        dev_addr,
        instance);
}

void hid_dispatch_report(
    uint8_t dev_addr,
    uint8_t instance,
    const uint8_t *report,
    uint16_t len)
{
    zuiki_on_hid_report(
        dev_addr,
        instance,
        report,
        len);
}

#include "zuiki.h"
#include "logger.h"

void zuiki_on_hid_mounted(
    uint8_t dev_addr,
    uint8_t instance,
    uint16_t vid,
    uint16_t pid)
{
    log_printf("Zuiki handler mounted: addr=%u inst=%u VID=%04X PID=%04X\r\n",
               dev_addr,
               instance,
               vid,
               pid);
}

void zuiki_on_hid_unmounted(
    uint8_t dev_addr,
    uint8_t instance)
{
    log_printf("Zuiki handler unmounted: addr=%u inst=%u\r\n",
               dev_addr,
               instance);
}

void zuiki_on_hid_report(
    uint8_t dev_addr,
    uint8_t instance,
    const uint8_t *report,
    uint16_t len)
{
    log_printf("Zuiki handler report: addr=%u inst=%u len=%u : ",
               dev_addr,
               instance,
               len);

    log_hex(report, len);
    log_printf("\r\n");
}

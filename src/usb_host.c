#include "usb_host.h"
#include "logger.h"
#include "hid_dispatch.h"
#include "oled.h"

#include "pico/stdlib.h"
#include "tusb.h"
#include "pio_usb.h"

void usb_host_init(void){
	pio_usb_configuration_t pio_cfg = PIO_USB_DEFAULT_CONFIG;
	pio_cfg.pin_dp = 12;

	tuh_configure(1, TUH_CFGID_RPI_PIO_USB_CONFIGURATION, &pio_cfg);
	
    tusb_init();
}

void usb_host_task(void){

        tud_task();
        tuh_task();

}

void tuh_mount_cb(uint8_t dev_addr) {
    uint16_t vid = 0;
    uint16_t pid = 0;

    tuh_vid_pid_get(dev_addr, &vid, &pid);

    log_printf("USB mounted: addr=%u VID=%04X PID=%04X\r\n",
               dev_addr, vid, pid);

//    oled_show_status("USB MOUNTED", "DEVICE FOUND", "VID/PID", "SEE LOG");
}

void tuh_umount_cb(uint8_t dev_addr) {
    log_printf("USB unmounted: addr=%u\r\n", dev_addr);

//    oled_show_status("USB UNMOUNTED", "WAITING USB...", "", "");
}

void tuh_hid_mount_cb(uint8_t dev_addr,
                      uint8_t instance,
                      uint8_t const *desc_report,
                      uint16_t desc_len) {
    (void) desc_report;

    uint16_t vid = 0;
    uint16_t pid = 0;
    tuh_vid_pid_get(dev_addr, &vid, &pid);

    uint8_t protocol = tuh_hid_interface_protocol(dev_addr, instance);

    log_printf("HID mounted\r\n");
    log_printf("  addr     = %u\r\n", dev_addr);
    log_printf("  instance = %u\r\n", instance);
    log_printf("  VID/PID  = %04X:%04X\r\n", vid, pid);
    log_printf("  protocol = %u\r\n", protocol);
    log_printf("  report descriptor length = %u\r\n", desc_len);

//    oled_show_status("HID MOUNTED", "ZUIKI READY", "REPORT WAIT", "");

    hid_dispatch_mount(dev_addr, instance, vid, pid);
                      	
    if (!tuh_hid_receive_report(dev_addr, instance)) {
        log_printf("ERROR: cannot request HID report\r\n");
    }
}

void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
    log_printf("HID unmounted: addr=%u instance=%u\r\n", dev_addr, instance);

//    oled_show_status("HID UNMOUNT", "WAITING USB...", "", "");

    hid_dispatch_unmount(dev_addr, instance);
}
void tuh_hid_report_received_cb(uint8_t dev_addr,
                                uint8_t instance,
                                uint8_t const *report,
                                uint16_t len) {

    uint16_t vid = 0;
    uint16_t pid = 0;
    tuh_vid_pid_get(dev_addr, &vid, &pid);

    log_printf("VID/PID=%04X:%04X\r\n", vid, pid);

    log_printf("HID report addr=%u inst=%u len=%u : ",
               dev_addr, instance, len);

    log_hex(report, len);
    log_printf("\r\n");

    hid_dispatch_report(dev_addr, instance, report, len);

//    oled_printf_line(0, "HID REPORT");
//    oled_printf_line(1, "VID %04X", vid);
//    oled_printf_line(2, "PID %04X", pid);
//    oled_printf_line(3, "LEN %u", len);
//    if (len >= 2) {
//        oled_printf_line(4, "%02X %02X %02X %02X",
//                         report[0],
//                         report[1],
//                         len > 2 ? report[2] : 0,
//                         len > 3 ? report[3] : 0);
//    }
//    oled_update();

    if (!tuh_hid_receive_report(dev_addr, instance)) {
        log_printf("ERROR: cannot request next HID report\r\n");
    }
}



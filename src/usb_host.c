#include "usb_host.h"
#include "logger.h"
#include "hid_dispatch.h"
#include "oled.h"

#include "pico/stdlib.h"
#include "tusb.h"
#include "pio_usb.h"
#include "usb_status.h"

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
	g_usb_status.usb_connected = true;
	g_usb_status.vid = vid;
	g_usb_status.pid = pid;

//    oled_show_status("USB MOUNTED", "DEVICE FOUND", "VID/PID", "SEE LOG");
}

void tuh_umount_cb(uint8_t dev_addr) {
	g_usb_status.usb_connected = false;
	g_usb_status.hid_connected = false;

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

    g_usb_status.hid_connected = true;

//    oled_show_status("HID MOUNTED", "ZUIKI READY", "REPORT WAIT", "");
    hid_dispatch_mount(dev_addr, instance, vid, pid);
	if (!tuh_hid_receive_report(dev_addr, instance)) {
    	log_printf("ERROR: cannot request HID report\r\n");
	}
}

void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
	g_usb_status.hid_connected = false;
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

	g_usb_status.report_len = len;

	memcpy(
    	g_usb_status.report,
    	report,
    	len > 64 ? 64 : len);
                                	
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
                                	
    hid_dispatch_report(dev_addr, instance, report, len);
	if (!tuh_hid_receive_report(dev_addr, instance)) {
    	log_printf("ERROR: cannot request next HID report\r\n");
	}
}



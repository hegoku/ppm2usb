#ifndef USB_HID_H
#define USB_HID_H

#include "usb.h"

#define USB_HID_BREQUEST_SET_IDLE 0x0a

struct usb_hid_interface_priv {
	unsigned char *report_descriptor;
	unsigned short report_descriptor_size;
};

#define USB_HID_INTERFACE_GET_PRIV(x) ((struct usb_hid_interface_priv*)((x)->priv))

void usb_hid_interface_class_request(struct usb_interface *interface, struct usb_standard_request_pack *buf);

#define USB_HID_INTERFACE_DEFINE(name, report, report_size) \
	struct usb_hid_interface_priv name##_usb_hid_inter_priv = {	\
		.report_descriptor = report,	\
		.report_descriptor_size = report_size	\
	};	\
	struct usb_interface name = {	\
		.class_request_handler=usb_hid_interface_class_request,	\
		.priv=&(name##_usb_hid_inter_priv)	\
	}

#endif
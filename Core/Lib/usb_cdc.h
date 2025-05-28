#ifndef USB_CDC_H
#define USB_CDC_H

#include "usb.h"

#define USB_CDC_DATA_BREQUEST_SET_LINE_CODING 0x20
#define USB_CDC_DATA_BREQUEST_GET_LINE_CODING 0x21
#define USB_CDC_DATA_BREQUEST_SET_CONTROL_LINE_STATE 0x22

struct usb_cdc_interface_priv {
	unsigned int dwDTERate;
	unsigned char bCharFormat;
	unsigned char bParityType;
	unsigned char bDataBits;
};

#define USB_CDC_INTERFACE_GET_PRIV(x) ((struct usb_cdc_interface_priv*)((x)->priv)

void usb_cdc_interface_class_request(struct usb_interface *interface, struct usb_standard_request_pack *buf);

void usb_cdc_data_interface_class_request(struct usb_interface *interface, struct usb_standard_request_pack *buf);

#define USB_CDC_INTERFACE_DEFINE(name) \
	struct usb_cdc_interface_priv name##_usb_cdc_inter_priv = {	\
		.dwDTERate = 0,	\
		.bCharFormat = 0,	\
		.bParityType = 0,	\
		.bDataBits = 0	\
	};	\
	struct usb_interface name = {	\
		.class_request_handler=usb_cdc_interface_class_request,	\
		.priv=&(name##_usb_cdc_inter_priv)	\
	}

#define USB_CDC_DATA_INTERFACE_DEFINE(name) \
	struct usb_interface name = {	\
		.class_request_handler=usb_cdc_data_interface_class_request,	\
	}

#endif
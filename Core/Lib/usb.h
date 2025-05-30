#ifndef USB_H
#define USB_H

#include "ring_buffer.h"

#define USB_BREQUEST_HOST_TO_DEVICE (0U << 7)
#define USB_BREQUEST_DEVICE_TO_HOST (1U << 7)

#define USB_BREQUEST_STANDARD (0U << 5)
#define USB_BREQUEST_CLASS (1U << 5)
#define USB_BREQUEST_VENDOR (2U << 5)

#define USB_BREQUEST_TO_DEVICE 0
#define USB_BREQUEST_TO_INTERFACE 1
#define USB_BREQUEST_TO_ENDPOINT 2
#define USB_BREQUEST_TO_OTHER 3

#define USB_BREQUEST_GET_STATUS 0
#define USB_BREQUEST_CLEAR_FEATURE 1
#define USB_BREQUEST_SET_FEATURE 2
#define USB_BREQUEST_GET_ADDR 4
#define USB_BREQUEST_SET_ADDR 5
#define USB_BREQUEST_GET_DESC 6
#define USB_BREQUEST_SET_DESC 7
#define USB_BREQUEST_GET_CONFIG 8
#define USB_BREQUEST_SET_CONFIG 9
#define USB_BREQUEST_GET_INTERFACE 10
#define USB_BREQUEST_SET_INTERFACE 11
#define USB_BREQUEST_SYNCH_FRAME 12
#define USB_BREQUEST_SET_IDLE 0x0a

#define USB_DESCRIPTOR_TYPE_DEVICE 0x1
#define USB_DESCRIPTOR_TYPE_CONFIG 0x2
#define USB_DESCRIPTOR_TYPE_STRING 0x3
#define USB_DESCRIPTOR_TYPE_INTERFACE 0x4
#define USB_DESCRIPTOR_TYPE_ENDPOINT 0x5
#define USB_DESCRIPTOR_TYPE_LIMITED 0x6
#define USB_DESCRIPTOR_TYPE_INTERFACE_ASSOCIATION 0x0B
#define USB_DESCRIPTOR_TYPE_HID 0x21
#define USB_DESCRIPTOR_TYPE_HID_REPORT 0x22

#define USB_ENDPOINT_DESC_DIR_IN (1U <<7)

enum USB_SETUP_STAGE {
    IDLE,
    DATA_IN,
    DATA_OUT,
    STATUS_IN,
    STATUS_OUT
};

struct usb_standard_request_pack {
    unsigned char bmRequest;
    unsigned char bRequst;
    unsigned char wValue[2];
    unsigned short wIndex;
    unsigned short wlength;
}__attribute__((packed));

struct usb_device {
    unsigned char address;
    volatile enum USB_SETUP_STAGE setup_stage;
    void(*ep0_setup_out_handler)(unsigned char* data, int size);
};

struct usb_interface {
	void (*class_request_handler_in)(struct usb_interface *interface, struct usb_standard_request_pack *buf);
    void (*class_request_handler_out)(struct usb_interface *interface, struct usb_standard_request_pack *buf);
	void *priv;
};

struct usb_endpoint {
    void(*rx_ct)(unsigned short id);
    void(*tx_ct)(unsigned short id);

    unsigned char rx_buf[64];

    unsigned char tx_remaining;
    unsigned char tx_data[256];
    struct ring_buffer tx_buf;
};

void usb_init();

void usb_send_endpoint_data(int id, unsigned char *buf, int size);
void usb_get_endpoint_data(int id, unsigned char *buf, int size);

void usb_set_endpoint_rx_valid(int id);
void usb_set_endpoint_tx_valid(int id);

extern struct usb_device usb_instance;

#endif
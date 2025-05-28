#include <stddef.h>
#include "usb.h"
#include "usb_hid.h"

void usb_hid_interface_class_request(struct usb_interface *interface, struct usb_standard_request_pack *buf)
{
	if (buf->bmRequest && USB_BREQUEST_DEVICE_TO_HOST) {
	} else {
		if (buf->bRequst==USB_HID_BREQUEST_SET_IDLE) {
			usb_send_endpoint_data(0, NULL, 0);
		}
	}
}
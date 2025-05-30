#include <stddef.h>
#include "usb.h"
#include "usb_hid.h"

void usb_hid_interface_class_request_in(struct usb_interface *interface, struct usb_standard_request_pack *buf)
{

}

void usb_hid_interface_class_request_out(struct usb_interface *interface, struct usb_standard_request_pack *buf)
{
	usb_instance.setup_stage = STATUS_IN;
	if (buf->bRequst==USB_HID_BREQUEST_SET_IDLE) {
		usb_send_endpoint_data(0, NULL, 0);
	}
}
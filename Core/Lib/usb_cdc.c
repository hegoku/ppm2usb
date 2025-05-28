#include "usb_cdc.h"

void usb_cdc_interface_class_request(struct usb_interface *interface, struct usb_standard_request_pack *buf)
{
	if (buf->bmRequest && USB_BREQUEST_DEVICE_TO_HOST) {
		if (buf->bRequst==USB_CDC_DATA_BREQUEST_GET_LINE_CODING) {
		}
	} else {
		if (buf->bRequst==USB_CDC_DATA_BREQUEST_SET_LINE_CODING) {
		}
	}
}

void usb_cdc_data_interface_class_request(struct usb_interface *interface, struct usb_standard_request_pack *buf)
{
	
}
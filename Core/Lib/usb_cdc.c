#include <stddef.h>
#include <string.h>
#include "usb_cdc.h"

static struct usb_interface *setup_out_interfance = NULL;

void usb_cdc_set_line_coding(unsigned char* data, int size)
{
	if (setup_out_interfance==NULL) return;
	struct usb_cdc_interface_priv *priv = USB_CDC_INTERFACE_GET_PRIV(setup_out_interfance);
	memcpy((unsigned char*)&(priv->line_config), data, size);
	usb_instance.ep0_setup_out_handler = NULL;
	setup_out_interfance = NULL;
	usb_instance.setup_stage = STATUS_IN;
	usb_send_endpoint_data(0, NULL, 0);
}

void usb_cdc_interface_class_request_in(struct usb_interface *interface, struct usb_standard_request_pack *buf)
{
	struct usb_cdc_interface_priv *priv = USB_CDC_INTERFACE_GET_PRIV(interface);
	if (buf->bRequst==USB_CDC_BREQUEST_GET_LINE_CODING) {
		usb_instance.setup_stage = DATA_IN;
		usb_send_endpoint_data(0, (unsigned char*)&(priv->line_config), sizeof(struct usb_cdc_interface_priv));
	}
}

void usb_cdc_interface_class_request_out(struct usb_interface *interface, struct usb_standard_request_pack *buf)
{
	if (buf->bRequst==USB_CDC_BREQUEST_SET_LINE_CODING) {
		setup_out_interfance = interface;
		usb_instance.ep0_setup_out_handler = usb_cdc_set_line_coding;
		usb_instance.setup_stage = DATA_OUT;
	} else if (buf->bRequst==USB_CDC_BREQUEST_SET_CONTROL_LINE_STATE) {
		usb_instance.setup_stage = STATUS_IN;
		usb_send_endpoint_data(0, NULL, 0);
	}
}

void usb_cdc_data_interface_class_request_in(struct usb_interface *interface, struct usb_standard_request_pack *buf)
{
	
}

void usb_cdc_data_interface_class_request_out(struct usb_interface *interface, struct usb_standard_request_pack *buf)
{
	
}
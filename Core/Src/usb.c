#include <stddef.h>
#include "usb.h"
#include "usb_hid.h"
#include "usb_cdc.h"
#include "usb_stm32f103x6.h"
#include "usb_desc.h"

USB_HID_INTERFACE_DEFINE(gamepad_hid_interface, usb_gamepad_rep_desc, sizeof(usb_gamepad_rep_desc));
USB_CDC_INTERFACE_DEFINE(cdc_inferface);
USB_CDC_DATA_INTERFACE_DEFINE(vcp_inferface);

static int is_get_address = 0;
static unsigned char dev_address;

static void ep0_rx(unsigned short id);
static void ep0_tx(unsigned short id);
static void ep1_ppm_tx(unsigned short id);
static void ep3_vcp_rx(unsigned short id);
static void ep4_vcp_tx(unsigned short id);

struct usb_interface *interface_list[] = {
    &gamepad_hid_interface,
    &cdc_inferface,
    &vcp_inferface
};

struct usb_endpoint endpoint_list[] = {
    {
        .rx_ct = ep0_rx,
        .tx_ct = ep0_tx
    },
    {
        .tx_ct = ep1_ppm_tx
    },
    {},
    {
        .rx_ct = ep3_vcp_rx
    },
    {
        .tx_ct = ep4_vcp_tx
    }
};

void(*ep0_setup_out_handler)(unsigned char* data, int size) = NULL;

void ep0_rx(unsigned short id)
{
    unsigned int reg = USB_EPnR(id);
    unsigned int receive_size = USB_EP_BUFF_DESC[id].rx_count & 0x1FF;
    usb_get_endpoint_data(0, endpoint_list[id].rx_buf, receive_size);
    if (reg & USB_EP_SETUP) {
        struct usb_standard_request_pack *buf = (struct usb_standard_request_pack*)(endpoint_list[id].rx_buf);

        if ((buf->bmRequest == (USB_BREQUEST_STANDARD | USB_BREQUEST_DEVICE_TO_HOST | USB_BREQUEST_TO_DEVICE))) {
            if (buf->bRequst == USB_BREQUEST_GET_DESC) {
                usb_setup_status = DATA_IN;
                switch (buf->wValue[1]) {
                    case USB_DESCRIPTOR_TYPE_DEVICE: //get device desc
                        usb_send_endpoint_data(0, usb_device_desc, sizeof(usb_device_desc));
                        break;
                    case USB_DESCRIPTOR_TYPE_CONFIG: //get config desc
                        if (buf->wlength > sizeof(usb_config_desc)) {
                            usb_send_endpoint_data(0, usb_config_desc, sizeof(usb_config_desc));
                        } else {
                            usb_send_endpoint_data(0, usb_config_desc, buf->wlength);
                        }
                        break;
                    case USB_DESCRIPTOR_TYPE_STRING: //get string desc
                        if (buf->wValue[0]==0) {
                            if (buf->wlength > sizeof(usb_string_desc_id)) {
                                usb_send_endpoint_data(0, usb_string_desc_id, sizeof(usb_string_desc_id));
                            } else {
                                usb_send_endpoint_data(0, usb_string_desc_id, buf->wlength);
                            }
                        } else {
                            if (buf->wlength > sizeof(usb_string_desc)) {
                                usb_send_endpoint_data(0, usb_string_desc, sizeof(usb_string_desc));
                            } else {
                                usb_send_endpoint_data(0, usb_string_desc, buf->wlength);
                            }
                        }
                        break;
                    case USB_DESCRIPTOR_TYPE_LIMITED: //get limited desc
                        usb_send_endpoint_data(0, usb_device_limited_desc, sizeof(usb_device_limited_desc));
                        break;
                    default:
                        usb_setup_status = IDLE;
                        break;
                }
            } else if (buf->bRequst == USB_BREQUEST_GET_STATUS) {
                
            }
        } else if (buf->bmRequest ==  (USB_BREQUEST_STANDARD | USB_BREQUEST_HOST_TO_DEVICE | USB_BREQUEST_TO_DEVICE)) {
            if (buf->bRequst==USB_BREQUEST_SET_ADDR) { // set device address
                dev_address = buf->wValue[0];
                is_get_address = 1;
            } else if (buf->bRequst==USB_BREQUEST_SET_CONFIG) {
            }
            usb_setup_status = STATUS_IN;
            usb_send_endpoint_data(0, NULL, 0);
        } else if (buf->bmRequest == (USB_BREQUEST_STANDARD | USB_BREQUEST_DEVICE_TO_HOST | USB_BREQUEST_TO_INTERFACE)) {
            if (buf->bRequst == USB_BREQUEST_GET_DESC) {
                usb_setup_status = DATA_IN;
                switch (buf->wValue[1]) {
                    case USB_DESCRIPTOR_TYPE_HID_REPORT:
                        if (buf->wlength > USB_HID_INTERFACE_GET_PRIV(interface_list[buf->wIndex])->report_descriptor_size) {
                            usb_send_endpoint_data(0, USB_HID_INTERFACE_GET_PRIV(interface_list[buf->wIndex])->report_descriptor, USB_HID_INTERFACE_GET_PRIV(interface_list[buf->wIndex])->report_descriptor_size);
                        } else {
                            usb_send_endpoint_data(0, USB_HID_INTERFACE_GET_PRIV(interface_list[buf->wIndex])->report_descriptor, buf->wlength);
                        }
                        break;
                    default:
                        usb_setup_status = IDLE;
                        break;
                }
            }
        } else if (buf->bmRequest == (USB_BREQUEST_STANDARD | USB_BREQUEST_HOST_TO_DEVICE | USB_BREQUEST_TO_INTERFACE)) {
            if (buf->bRequst==USB_BREQUEST_SET_INTERFACE) {
            }
        } else if (buf->bmRequest == (USB_BREQUEST_CLASS | USB_BREQUEST_DEVICE_TO_HOST | USB_BREQUEST_TO_INTERFACE)) {
            interface_list[buf->wIndex]->class_request_handler_in(interface_list[buf->wIndex], buf);
        } else if (buf->bmRequest == (USB_BREQUEST_CLASS | USB_BREQUEST_HOST_TO_DEVICE | USB_BREQUEST_TO_INTERFACE)) {
            interface_list[buf->wIndex]->class_request_handler_out(interface_list[buf->wIndex], buf);
        }
    } else {
        if (usb_setup_status == STATUS_OUT) {
            usb_setup_status = IDLE;
        } else if (usb_setup_status == DATA_OUT) {
            if (ep0_setup_out_handler) {
                ep0_setup_out_handler(endpoint_list[id].rx_buf, receive_size);
            }
        }
    }
    USB_EPnR(id) = (USB_EPnR(id) & USB_EPREG_MASK) | USB_EP_RX_STALL;
}

void ep0_tx(unsigned short id)
{
    if (usb_setup_status==DATA_IN) {
        _usb_endpoint_send(id);
        if (endpoint_list[id].tx_remaining==0) {
            usb_setup_status = IDLE;
        }
    } else if (usb_setup_status==STATUS_IN) {
        if (is_get_address) {
            USB->DADDR |= dev_address;
            is_get_address = 1;
        }
        usb_setup_status = IDLE;
    }
   
}

void ep1_ppm_tx(unsigned short id)
{
    unsigned int reg = USB_EPnR(id);
}

void ep3_vcp_rx(unsigned short id)
{

}

void ep4_vcp_tx(unsigned short id)
{

}
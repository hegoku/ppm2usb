#include <stddef.h>
#include "usb.h"
#include "usb_stm32f103x6.h"
#include "usb_desc.h"

static int is_get_address = 0;
static unsigned char dev_address;

static void ep0_rx(unsigned short id);
static void ep0_tx(unsigned short id);
static void ep1_ppm_tx(unsigned short id);

struct usb_endpoint endpoint_list[4] = {
    {
        .rx_ct = ep0_rx,
        .tx_ct = ep0_tx
    },
    {
        .tx_ct = ep1_ppm_tx
    }
};

void ep0_rx(unsigned short id)
{
    unsigned int reg = USB_EPnR(id);
    if (reg & USB_EP_SETUP) {
        usb_get_endpoint_data(0, endpoint_list[id].rx_buf, 8);
        struct usb_standard_request_pack *buf = (struct usb_standard_request_pack*)(endpoint_list[id].rx_buf);

        if ((buf->bmRequest == (USB_BREQUEST_STANDARD | USB_BREQUEST_DEVICE_TO_HOST | USB_BREQUEST_TO_DEVICE))) {
            if (buf->bRequst == USB_BREQUEST_GET_DESC) {
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
                }
            } else if (buf->bRequst == USB_BREQUEST_GET_STATUS) {
                
            }            
        }  else if (buf->bmRequest == (USB_BREQUEST_STANDARD | USB_BREQUEST_DEVICE_TO_HOST | USB_BREQUEST_TO_INTERFACE) && buf->bRequst == USB_BREQUEST_GET_DESC) {
            switch (buf->wValue[1]) {
            case USB_DESCRIPTOR_TYPE_HID_REPORT:
                usb_send_endpoint_data(0, usb_gamepad_rep_desc, sizeof(usb_gamepad_rep_desc));
                break;
            }
        } else if (buf->bmRequest ==  (USB_BREQUEST_STANDARD | USB_BREQUEST_HOST_TO_DEVICE | USB_BREQUEST_TO_DEVICE)) {
            if (buf->bRequst==USB_BREQUEST_SET_ADDR) { // set device address
                dev_address = buf->wValue[0];
                is_get_address = 1;
            } else if (buf->bRequst==USB_BREQUEST_SET_CONFIG) {
            }
            usb_send_endpoint_data(0, NULL, 0);
        } else if (buf->bmRequest == (USB_BREQUEST_CLASS | USB_BREQUEST_HOST_TO_DEVICE | USB_BREQUEST_TO_INTERFACE) && buf->bRequst==USB_BREQUEST_SET_IDLE) { //类限定请求,设置空速率
            usb_send_endpoint_data(0, NULL, 0);
        }
    }
    USB_EPnR(id) = (USB_EPnR(id) & USB_EPREG_MASK) | USB_EP_RX_STALL;
}

void ep0_tx(unsigned short id)
{
    if (is_get_address) {
        USB->DADDR |= dev_address;
        is_get_address = 1;
    }
    _usb_endpoint_send(id);
}

void ep1_ppm_tx(unsigned short id)
{
    unsigned int reg = USB_EPnR(id);
}
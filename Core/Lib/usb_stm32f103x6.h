#ifndef USB_STM32F103x6_H
#define USB_STM32F103x6_H

#include "stm32f103x6.h"
#include "ring_buffer.h"

struct usb_ep_config {
    unsigned int tx_addr;
    unsigned int tx_count;
    unsigned int rx_addr;
    unsigned int rx_count;
}__attribute__((packed));

struct usb_endpoint {
    void(*rx_ct)(unsigned short id);
    void(*tx_ct)(unsigned short id);

    unsigned char rx_buf[64];

    unsigned char tx_remaining;
    unsigned char tx_data[256];
    struct ring_buffer tx_buf;
};

#define USB_EP_BUFF_DESC ((struct usb_ep_config*)USB_PMAADDR)
#define USB_EPnR(x) (((volatile unsigned int*)USB_EP0R)[x])

#define USB_GET_EP_TX_ADDR(id) ((unsigned short *) ((USB_EP_BUFF_DESC[id].tx_addr<<1) + USB_PMAADDR))
#define USB_GET_EP_RX_ADDR(id) ((unsigned short *) ((USB_EP_BUFF_DESC[id].rx_addr<<1) + USB_PMAADDR))

void usb_it_reset();
void usb_it_ct();

void _usb_endpoint_send(int id);

#endif
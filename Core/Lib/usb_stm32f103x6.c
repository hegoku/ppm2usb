#include "usb.h"
#include "usb_stm32f103x6.h"

extern struct usb_endpoint endpoint_list[4];

void usb_init()
{
    unsigned int tmp = RCC->CFGR & ~RCC_CFGR_USBPRE_Msk;
    RCC->CFGR = tmp;

    RCC->APB1ENR |= RCC_APB1ENR_USBEN;

    __NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
    // __NVIC_EnableIRQ(USBWakeUp_IRQn);

    USB->CNTR |= USB_CNTR_RESETM | USB_CNTR_CTRM;
    tmp = USB->CNTR & ~USB_CNTR_PDWN;
    USB->CNTR = tmp;
    tmp = USB->CNTR & ~USB_CNTR_FRES;
    USB->CNTR = tmp;
    USB->ISTR = 0;
}

void usb_it_reset()
{
    USB->DADDR |= USB_DADDR_EF;
    USB->EP0R = USB_EP_CONTROL | USB_EP_RX_VALID | USB_EP_TX_NAK | USB_EP_KIND;
    USB_EP_BUFF_DESC[0].rx_count = (1UL<<10) | 0x8000;
    USB_EP_BUFF_DESC[0].rx_addr = 0x20;
    USB_EP_BUFF_DESC[0].tx_addr = (0x20+64);

    USB->EP1R = USB_EP_INTERRUPT | USB_EP_TX_NAK | 1;
    USB_EP_BUFF_DESC[1].rx_count = (1UL<<10) | 0x8000;
    USB_EP_BUFF_DESC[1].rx_addr = (USB_EP_BUFF_DESC[0].tx_addr+64);
    USB_EP_BUFF_DESC[1].tx_addr = (USB_EP_BUFF_DESC[1].rx_addr+64);

    USB->EP2R = USB_EP2R_EP_TYPE | USB_EP2R_STAT_RX | USB_EP2R_STAT_TX | 2;
    USB_EP_BUFF_DESC[2].rx_count = (1UL<<10) | 0x8000;
    USB_EP_BUFF_DESC[2].rx_addr = (USB_EP_BUFF_DESC[1].tx_addr+64);
    USB_EP_BUFF_DESC[2].tx_addr = (USB_EP_BUFF_DESC[2].rx_addr+64);

    for (int i=0;i<4;i++) {
        ring_buffer_clear(&endpoint_list[i].tx_buf);
        endpoint_list[i].tx_remaining = 0;
        endpoint_list[i].tx_buf.data = endpoint_list[i].tx_data;
        endpoint_list[i].tx_buf.size = 256;
    }
}

void usb_it_ct()
{
    unsigned short id = (unsigned short)(USB->ISTR & 0xF);
    unsigned int reg = USB_EPnR(id);
    if (reg & USB_EP_CTR_RX) {
        if (endpoint_list[id].rx_ct) {
            endpoint_list[id].rx_ct(id);
        }
        USB_EPnR(id) = (USB_EPnR(id) & USB_EPREG_MASK) & ~USB_EP_CTR_RX;
    }
    if (reg & USB_EP_CTR_TX) {
        if (endpoint_list[id].tx_ct) {
            endpoint_list[id].tx_ct(id);
        }
        USB_EPnR(id) = (USB_EPnR(id) & USB_EPREG_MASK) & ~USB_EP_CTR_TX;
    }
}

void _usb_endpoint_send(int id)
{
    if ((USB_EPnR(id) & USB_EP_TX_VALID) ==0) {
        return;
    }
    if (endpoint_list[id].tx_remaining==0) return;
    int size = 0;
    unsigned char buf[64];
    unsigned short *send = USB_GET_EP_TX_ADDR(id);
    unsigned short *data = (unsigned short*)buf;

    size = ring_buffer_pop(&endpoint_list[id].tx_buf, buf, 64);
    for (int i=0;i<(size/2);i++) {
        *send = data[i];
        send+=2;
    }

    if (size%2) {
        *send = (unsigned int)(buf[size-1]);
    }
    USB_EP_BUFF_DESC[id].tx_count = size;
    USB_EPnR(id) = (USB_EPnR(id) & USB_EPREG_MASK) | USB_EP_TX_STALL;

    if (RING_BUFFER_IS_EMPTY(endpoint_list[id].tx_buf)) {
        endpoint_list[id].tx_remaining = 0;
    }
}

void usb_send_endpoint_data(int id, unsigned char *buf, int size)
{
    ring_buffer_clear(&endpoint_list[id].tx_buf);
    ring_buffer_put(&endpoint_list[id].tx_buf, buf, size);
    endpoint_list[id].tx_remaining = 1;
    _usb_endpoint_send(id);
}

void usb_get_endpoint_data(int id, unsigned char *buf, int size)
{
    unsigned short *read = USB_GET_EP_RX_ADDR(id);
    unsigned short *data = (unsigned short*)buf;

    for (int i=0;i<(size/2);i++) {
        data[i] = *read;
        read += 2;
    }
}

void use_send_data(int id, unsigned char *buf, int size)
{

}
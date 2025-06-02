/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f103x6.h"
#include "usb.h"
#include "usb_stm32f103x6.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/* USER CODE BEGIN 1 */

// static unsigned char ep_rx_buf[64] = {0};
// static int is_get_address = 0;
// static unsigned char dev_address;

// void handleZero()
// {
//   unsigned int reg = USB->EP0R;
//   if (reg & USB_EP0R_CTR_RX) {//out/Setup transaction
//     if ((USB_EP_BUFF_DESC[0].rx_count & 0x3FF) == 8) {
//       getEndpointData(0, ep_rx_buf, 8);
//       struct usb_standard_request_pack *buf = (struct usb_standard_request_pack*)(ep_rx_buf);
//       if ((buf->bmRequest == (USB_BREQUEST_STANDARD | USB_BREQUEST_DEVICE_TO_HOST | USB_BREQUEST_TO_DEVICE)) && buf->bRequst == USB_BREQUEST_GET_DESC) {
//         switch (buf->wValue[1]) {
//           case USB_DESCRIPTOR_TYPE_DEVICE: //get device desc
//             sendEndpointData(0, usb_device_desc, sizeof(usb_device_desc));
//             break;
//           case USB_DESCRIPTOR_TYPE_CONFIG: //get config desc
//             if (buf->wlength == 0xff) {
//               sendEndpointData(0, usb_config_desc, sizeof(usb_config_desc));
//             } else {
//               sendEndpointData(0, usb_config_desc, buf->wlength);
//             }
//             break;
//           case USB_DESCRIPTOR_TYPE_STRING: //get string desc
//             if (buf->wValue[0]==0) {
//               if (buf->wlength == 0xff) {
//                 sendEndpointData(0, usb_string_desc_id, sizeof(usb_string_desc_id));
//               } else {
//                 sendEndpointData(0, usb_string_desc_id, buf->wlength);
//               }
//             } else {
//               if (buf->wlength == 0xff) {
//                 sendEndpointData(0, usb_string_desc, sizeof(usb_string_desc));
//               } else {
//                 sendEndpointData(0, usb_string_desc, buf->wlength);
//               }
//             }
//             break;
//           case USB_DESCRIPTOR_TYPE_LIMITED: //get limited desc
//             sendEndpointData(0, usb_device_limited_desc, sizeof(usb_device_limited_desc));
//             break;
//         }
//         reg |= USB_EP0R_STAT_TX_0;
//       } else if (buf->bmRequest ==  (USB_BREQUEST_STANDARD | USB_BREQUEST_HOST_TO_DEVICE | USB_BREQUEST_TO_DEVICE)) {
//         if (buf->bRequst==USB_BREQUEST_SET_ADDR) { // set device address
//           dev_address = buf->wValue[0];
//           is_get_address = 1;
//         } else if (buf->bRequst==USB_BREQUEST_SET_CONFIG) {
//           unsigned char d[] = {0x1, 0x2, 0x3, 0x4, 0x0};
//           sendEndpointData(1, d, sizeof(d));
//         }

//         sendEndpointData(0, NULL, 0);
//         reg |= USB_EP0R_STAT_TX_0;
//       } else if (buf->bmRequest == (USB_BREQUEST_STANDARD | USB_BREQUEST_DEVICE_TO_HOST | USB_BREQUEST_TO_INTERFACE) && buf->bRequst == USB_BREQUEST_GET_DESC) {
//         switch (buf->wValue[1]) {
//           case USB_DESCRIPTOR_TYPE_HID_REPORT:
//             sendEndpointData(0, usb_gamepad_rep_desc, sizeof(usb_gamepad_rep_desc));
//             break;
//         }
//         reg |= USB_EP0R_STAT_TX_0;
//       } else if (buf->bmRequest == (USB_BREQUEST_CLASS | USB_BREQUEST_HOST_TO_DEVICE | USB_BREQUEST_TO_INTERFACE) && buf->bRequst==USB_BREQUEST_SET_IDLE) { //类限定请求,设置空速率
//         sendEndpointData(0, NULL, 0);
//         reg |= USB_EP0R_STAT_TX_0;
//       }
//     }
//   } else if (reg & USB_EP0R_CTR_TX) { //in transaction
//     if (is_get_address) {
//       USB->DADDR |= dev_address;
//       is_get_address = 1;
//     }
//   }
//   reg |= USB_EP0R_STAT_RX;
//   reg  &= ~(USB_EP0R_EP_TYPE_0 | USB_EP0R_EP_KIND);
//   USB->EP0R ^= reg;
// }

// static unsigned char x=0,y=255,rx=150;
// void handleEndpoint(unsigned short id)
// {
//   unsigned int reg = USB_EPnR(id);
//   if (reg & USB_EP0R_CTR_RX) {
//     reg |= USB_EP0R_STAT_RX_0;
//     reg &= ~id;
//     reg &= ~USB_EP1R_EP_TYPE;
//   }
//   if (reg & USB_EP0R_CTR_TX) {
//     reg |= USB_EP0R_STAT_TX_0;
//     reg &= ~id;
//     reg &= ~USB_EP1R_EP_TYPE;
//     unsigned char d[] = {x++, y++, rx++, 0x4, 0x0};
//     sendEndpointData(1, d, sizeof(d));
//   }
//   USB_EPnR(id) ^= reg;
// }

void USB_LP_IRQHandler(void)
{
  if (USB->ISTR & USB_ISTR_RESET) {
    // is_get_address = 0;
    usb_it_reset();
    // USB->DADDR |= USB_DADDR_EF;
    // USB->EP0R = USB_EP0R_EP_TYPE_0 | USB_EP0R_STAT_RX | USB_EP0R_STAT_TX | USB_EP0R_EP_KIND;
    // USB_EP_BUFF_DESC[0].rx_count = (1UL<<10) | 0x8000;
    // USB_EP_BUFF_DESC[0].rx_addr = 0x20;
    // USB_EP_BUFF_DESC[0].tx_addr = (0x20+64);

    // USB->EP1R = USB_EP1R_EP_TYPE | USB_EP0R_STAT_TX | USB_EP1R_STAT_RX | 1;
    // USB_EP_BUFF_DESC[1].rx_count = (1UL<<10) | 0x8000;
    // USB_EP_BUFF_DESC[1].rx_addr = (USB_EP_BUFF_DESC[0].tx_addr+64);
    // USB_EP_BUFF_DESC[1].tx_addr = (USB_EP_BUFF_DESC[1].rx_addr+64);

    // USB->EP2R = USB_EP2R_EP_TYPE | USB_EP2R_STAT_RX | USB_EP2R_STAT_TX | 2;
    // USB_EP_BUFF_DESC[2].rx_count = (1UL<<10) | 0x8000;
    // USB_EP_BUFF_DESC[2].rx_addr = (USB_EP_BUFF_DESC[1].tx_addr+64);
    // USB_EP_BUFF_DESC[2].tx_addr = (USB_EP_BUFF_DESC[2].rx_addr+64);
    USB->ISTR =  USB->ISTR & ~USB_ISTR_RESET;
  }
  // if (USB->ISTR & USB_ISTR_SUSP) {
  //   USB->ISTR =  USB->ISTR & ~USB_ISTR_SUSP;
  //   USB->CNTR |= USB_CNTR_FSUSP;
  // }
  // if (USB->ISTR & USB_ISTR_ESOF) {
  //   USB->ISTR =  USB->ISTR & ~USB_ISTR_ESOF;
  // }
  if (USB->ISTR & USB_ISTR_CTR) {
    usb_it_ct();
    // unsigned short id = (unsigned short)(USB->ISTR & 0xF);
    // if (id==0) {
    //   handleZero();
    // } else {
    //   handleEndpoint(id);
    // }
    USB->ISTR =  USB->ISTR & ~USB_ISTR_CTR;
  }
}

void USBWakeUp_IRQHandler(void)
{
  USB->ISTR =  USB->ISTR & ~USB_ISTR_WKUP;
  USB->CNTR = USB->CNTR & ~USB_CNTR_FSUSP;
}

void TIM2_IRQHandler(void)
{
  // unsigned short cycle = TIM2->CCR1;
  // unsigned short width = TIM2->CCR2;
  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}
/* USER CODE END 1 */

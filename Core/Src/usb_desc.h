#ifndef SRC_USB_DESC_H
#define SRC_USB_DESC_H

#include "usb.h"

static unsigned char usb_device_desc[] = {
  0x12, //描述符长度18B
  USB_DESCRIPTOR_TYPE_DEVICE, //设备描述符类型
  0x00, 0x02, //USB版本 1.10
  0, //设备类代码
  0, //设备子类代码
  0, //设备协议代码
  0x40, //endpoint0最大包长
  0x96, 0x03, //厂商ID
  0x15, 0x1, //产品ID
  0x00, 0x01, //设备版本号 01.00
  0x01, //制造商字符串索引
  0x02, //产品字符串索引
  0x03, //序列号字符串索引
  0x01 //可能的配置描述符数量
};

static unsigned char usb_gamepad_rep_desc[] = {
  0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
  0x09, 0x05,                    // USAGE (Game Pad)
  0xa1, 0x01,                    // COLLECTION (Application)
  0xa1, 0x02,                    //   COLLECTION (Logical)
  0x75, 0x08,                    //     REPORT_SIZE (8)
  0x95, 0x04,                    //     REPORT_COUNT (4)
  0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
  0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
  0x35, 0x00,                    //     PHYSICAL_MINIMUM (0)
  0x46, 0xff, 0x00,              //     PHYSICAL_MAXIMUM (255)
  0x09, 0x30,                    //     USAGE (X)
  0x09, 0x31,                    //     USAGE (Y)
  0x09, 0x33,                    //     USAGE (Rx)
  0x09, 0x34,                    //     USAGE (Ry)
  0x81, 0x02,                    //     INPUT (Data,Var,Abs)
  0x75, 0x01,                    //     REPORT_SIZE (1)
  0x95, 0x04,                    //     REPORT_COUNT (4)
  0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
  0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
  0x35, 0x00,                    //     PHYSICAL_MINIMUM (0)
  0x45, 0x01,                    //     PHYSICAL_MAXIMUM (1)
  0x05, 0x09,                    //     USAGE_PAGE (Button)
  0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
  0x29, 0x04,                    //     USAGE_MAXIMUM (Button 4)
  0x81, 0x02,                    //     INPUT (Data,Var,Abs)
  0x75, 0x01,                    //     REPORT_SIZE (1)
  0x95, 0x04,                    //     REPORT_COUNT (4)
  0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
  0xc0,                          //     END_COLLECTION
  0xc0                           // END_COLLECTION
};

static unsigned char usb_config_desc[] = {
//配置描述符
  0x09, //配置描述符长度 9B
  USB_DESCRIPTOR_TYPE_CONFIG, //类型为配置
  0x29, 0x00, //整个配置描述符集总长度
  0x1, //接口数量
  0x1, //被SET_CONFIGURATION请求用作参数来选定
  0x4, //描述该配置的字符串描述符索引
  0x80, //供电模式 总线供电
  0xFA, //最大功耗 以2mA为单位, 500mA
//接口描述符
  0x09, //长度
  USB_DESCRIPTOR_TYPE_INTERFACE, //类型为接口
  0x0, //接口编号
  0x0, //备用接口编号
  0x1, //该接口endpoint数
  0x3, //接口类型 HID
  0x0, //接口子类型
  0x0, //协议代码
  0x05, //字符串索引
//HID描述符
  0x09, //长度
  USB_DESCRIPTOR_TYPE_HID, //类型为HID
  0x12, 0x01, //bcdHID HID规范版本号的BCD码，此描述符所用版本为1.12
  0x00, //bCountryCode 硬件目的国家的识别号码。不启用此功能则为0
  0x01, //bNumDescriptors 支持的附属描述符数目。最小值为1：HID类至少有个报表描述符
  USB_DESCRIPTOR_TYPE_HID_REPORT, //bDescriptorType 类别描述符的类型。只有报表描述符为0x22；还有个实体描述符则为0x23
  sizeof(usb_gamepad_rep_desc), 0x00, //wDescriptorLength 报表描述符总长度
//endpoint desc in
  0x7, //描述符长度
  USB_DESCRIPTOR_TYPE_ENDPOINT, //类型为端点
  USB_ENDPOINT_DESC_DIR_IN | 0x1, //端点方向和地址
  0x3, //中断传输
  0x40, 0x00, //端点最大包长度
  0x14, //主机查询间隔 20ms
//endpoint desc out
  // 0x7, //描述符长度
  // USB_DESCRIPTOR_TYPE_ENDPOINT, //类型为端点
  // 0x2, //端点方向和地址
  // 0x3, //中断传输
  // 0x40, 0x00, //端点最大包长度
  // 0x14 //主机查询间隔 20ms
};

static unsigned char usb_device_limited_desc[] = {
  0xA, //描述符长度 10B
  USB_DESCRIPTOR_TYPE_LIMITED, //描述符类型(设备限定描述符)
  0x2, 0x0, //USB规范版本 2.0
  0x0, //设备类
  0x0, //子类代码
  0x0, //协议代码
  0x40, //Endpoint 0最大包大小（64B)
  0x1, //其他速度配置数量1个
  0x0 //保留字段
};

static unsigned char usb_string_desc[] = {
  0x10, //描述符长度
  USB_DESCRIPTOR_TYPE_STRING, //类型为字符串
  'p', 0, 'p', 0, 'w', 0, '2', 0, 'u', 0, 's', 0, 'b', 0x0
};

static unsigned char usb_string_desc_id[] = {
  0x4,
  USB_DESCRIPTOR_TYPE_STRING, //类型为字符串
  0x9, 0x4 //支持语言ID 1(英文)
};

#endif
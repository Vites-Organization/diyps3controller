/*
 Copyright (c) 2013 Mathieu Laurendeau
 License: GPLv3
 */

#ifndef _ADAPTER_PROTOCOL_H_
#define _ADAPTER_PROTOCOL_H_

#define BYTE_NO_PACKET   0x00
#define BYTE_TYPE        0x11
#define BYTE_STATUS      0x22
#define BYTE_START_SPOOF 0x33
#define BYTE_SPOOF_DATA  0x44
#define BYTE_SEND_REPORT 0xff

#define BYTE_TYPE_JOYSTICK   0x00
#define BYTE_TYPE_X360       0x01
#define BYTE_TYPE_SIXAXIS    0x02
#define BYTE_TYPE_PS2        0x03
#define BYTE_TYPE_XBOX       0x04

#define BYTE_STATUS_NSPOOFED 0x00
#define BYTE_STATUS_SPOOFED  0x01

#define BYTE_LEN_1_BYTE 0x01

#endif

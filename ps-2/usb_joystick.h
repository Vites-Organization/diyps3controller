/*
    Diy PS3 controller
    Copyright (C) 2010  Mathieu Laurendeau

    Original code from the Keyboard example for Teensy USB Development Board
    http://www.pjrc.com/teensy/usb_keyboard.html
    Copyright (c) 2008 PJRC.COM, LLC

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef usb_serial_h__
#define usb_serial_h__

#include <stdint.h>

void usb_joystick_raz(void);
void usb_joystick_raz_buttons(void);
void usb_joystick_raz_axis(void);

void usb_init(void);			// initialize everything
uint8_t usb_configured(void);		// is the USB port configured

void usb_joystick_press(uint16_t);
void usb_joystick_release(uint16_t);
void usb_joystick_press_dir(uint16_t);
void usb_joystick_move_x(uint8_t);
void usb_joystick_move_y(uint8_t);
void usb_joystick_move_zrz(uint8_t, uint8_t);
int8_t usb_joystick_send(void);

// This file does not include the HID debug functions, so these empty
// macros replace them with nothing, so users can compile code that
// has calls to these functions.
#define usb_debug_putchar(c)
#define usb_debug_flush_output()

#define UP_BUTTON             0x0000
#define UP_R_BUTTON           0x0001
#define RIGHT_BUTTON          0x0002
#define DOWN_R_BUTTON         0x0003
#define DOWN_BUTTON           0x0004
#define DOWN_L_BUTTON         0x0005
#define LEFT_BUTTON           0x0006
#define UP_L_BUTTON           0x0007

#define NO_BUTTON             0x0008

#define RIGHT_HAT_BUTTON      0x8000
#define LEFT_HAT_BUTTON       0x4000
#define START_BUTTON          0x2000
#define SELECT_BUTTON         0x1000
#define R2_BUTTON             0x0800
#define L2_BUTTON             0x0400
#define R1_BUTTON             0x0200
#define L1_BUTTON             0x0100
#define TRIANGLE_BUTTON       0x0080
#define CIRCLE_BUTTON         0x0040
#define CROSS_BUTTON          0x0020
#define SQUARE_BUTTON         0x0010

#define ACTION_VW_IDLE          128
#define ACTION_VW_DEAD_ZONE     25
#define ACTION_VW_LOW           0
#define ACTION_VW_HIGH          255
#define ACTION_VW_MAXVECTOR     126
#define ACTION_VW_MINVECTOR     0

// Everything below this point is only intended for usb_serial.c
#ifdef USB_SERIAL_PRIVATE_INCLUDE
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define EP_TYPE_CONTROL			0x00
#define EP_TYPE_BULK_IN			0x81
#define EP_TYPE_BULK_OUT		0x80
#define EP_TYPE_INTERRUPT_IN		0xC1
#define EP_TYPE_INTERRUPT_OUT		0xC0
#define EP_TYPE_ISOCHRONOUS_IN		0x41
#define EP_TYPE_ISOCHRONOUS_OUT		0x40

#define EP_SINGLE_BUFFER		0x02
#define EP_DOUBLE_BUFFER		0x06

#define EP_SIZE(s)	((s) == 64 ? 0x30 :	\
			((s) == 32 ? 0x20 :	\
			((s) == 16 ? 0x10 :	\
			             0x00)))

#define MAX_ENDPOINT		4

#define LSB(n) (n & 255)
#define MSB(n) ((n >> 8) & 255)

#if defined(__AVR_AT90USB162__)
#define HW_CONFIG() 
#define PLL_CONFIG() (PLLCSR = ((1<<PLLE)|(1<<PLLP0)))
#define USB_CONFIG() (USBCON = (1<<USBE))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#elif defined(__AVR_ATmega32U4__)
#define HW_CONFIG() (UHWCON = 0x01)
#define PLL_CONFIG() (PLLCSR = 0x12)
#define USB_CONFIG() (USBCON = ((1<<USBE)|(1<<OTGPADE)))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#elif defined(__AVR_AT90USB646__)
#define HW_CONFIG() (UHWCON = 0x81)
#define PLL_CONFIG() (PLLCSR = 0x1A)
#define USB_CONFIG() (USBCON = ((1<<USBE)|(1<<OTGPADE)))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#elif defined(__AVR_AT90USB1286__)
#define HW_CONFIG() (UHWCON = 0x81)
#define PLL_CONFIG() (PLLCSR = 0x16)
#define USB_CONFIG() (USBCON = ((1<<USBE)|(1<<OTGPADE)))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#endif

// standard control endpoint request types
#define GET_STATUS			0
#define CLEAR_FEATURE			1
#define SET_FEATURE			3
#define SET_ADDRESS			5
#define GET_DESCRIPTOR			6
#define GET_CONFIGURATION		8
#define SET_CONFIGURATION		9
#define GET_INTERFACE			10
#define SET_INTERFACE			11
// HID (human interface device)
#define HID_GET_REPORT			1
#define HID_GET_IDLE			2
#define HID_GET_PROTOCOL		3
#define HID_SET_REPORT			9
#define HID_SET_IDLE			10
#define HID_SET_PROTOCOL		11
// CDC (communication class device)
#define CDC_SET_LINE_CODING		0x20
#define CDC_GET_LINE_CODING		0x21
#define CDC_SET_CONTROL_LINE_STATE	0x22
#endif
#endif

/*
 * controller.h
 *
 *  Created on: 13 nov. 2013
 *      Author: matlo
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#define DEFAULT_MAX_AXIS_VALUE MAX_AXIS_VALUE_8BITS

#define MAX_AXIS_VALUE_8BITS 255
#define MAX_AXIS_VALUE_10BITS 1023
#define MAX_AXIS_VALUE_16BITS 65535

typedef enum
{
  C_TYPE_JOYSTICK = 0,
  C_TYPE_360_PAD,
  C_TYPE_SIXAXIS,
  C_TYPE_PS2_PAD,
  C_TYPE_XBOX_PAD,
  C_TYPE_DS4,
  C_TYPE_GPP,
  C_TYPE_DEFAULT,
  C_TYPE_MAX
} e_controller_type;

typedef enum
{
  rel_axis_0 = 0,
  rel_axis_1,
  rel_axis_2,
  rel_axis_3,
  rel_axis_lstick_x = rel_axis_0,
  rel_axis_lstick_y = rel_axis_1,
  rel_axis_rstick_x = rel_axis_2,
  rel_axis_rstick_y = rel_axis_3,
  rel_axis_4,
  rel_axis_5,
  rel_axis_6,
  rel_axis_7,
  rel_axis_max = rel_axis_7,
  abs_axis_0,
  abs_axis_1,
  abs_axis_2,
  abs_axis_3,
  abs_axis_4,
  abs_axis_5,
  abs_axis_6,
  abs_axis_7,
  abs_axis_8,
  abs_axis_9,
  abs_axis_10,
  abs_axis_11,
  abs_axis_12,
  abs_axis_13,
  abs_axis_14,
  abs_axis_15,
  abs_axis_16,
  abs_axis_17,
  abs_axis_max = abs_axis_17,
  AXIS_MAX,
} e_controller_axis_index;

typedef struct
{
  char* portname;
  char* ip;
  unsigned short port;
  int fd;
  e_controller_type type;
  int event;
  int axis[AXIS_MAX];
  int change;
  int send_command;
  int ts_axis[AXIS_MAX][2]; //issue 15
} s_controller;

void controller_init_type();
inline s_controller* get_controller(unsigned char index);

int get_min_refresh_period(e_controller_type type);
int get_default_refresh_period(e_controller_type type);

inline int get_max_signed(e_controller_type, int);
inline int get_max_unsigned(e_controller_type, int);
inline int get_mean_unsigned(e_controller_type, int);
inline double get_axis_scale(e_controller_type, int);

void set_axis_value(unsigned char c, int axis, int value);

inline int clamp(int, int, int);

typedef struct {
    int value;
    int index;
} s_axis_index;

s_axis_index get_axis_index_from_name(const char*);

void controller_dump_state(s_controller* c);

#endif /* CONTROLLER_H_ */

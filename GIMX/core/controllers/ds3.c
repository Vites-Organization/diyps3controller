/*
 * ds3.c
 *
 *  Created on: 13 nov. 2013
 *      Author: matlo
 */

#include "controllers/ds3.h"
#include <report.h>
#include <string.h>

static const char *axis_name[AXIS_MAX] = {
    [sa_lstick_x] = "lstick x",
    [sa_lstick_y] = "lstick y",
    [sa_rstick_x] = "rstick x",
    [sa_rstick_y] = "rstick y",
    [sa_acc_x] = "acc x",
    [sa_acc_y] = "acc y",
    [sa_acc_z] = "acc z",
    [sa_gyro] = "gyro",
    [sa_select] = "select",
    [sa_start] = "start",
    [sa_ps] = "PS",
    [sa_up] = "up",
    [sa_right] = "right",
    [sa_down] = "down",
    [sa_left] = "left",
    [sa_triangle] = "triangle",
    [sa_circle] = "circle",
    [sa_cross] = "cross",
    [sa_square] = "square",
    [sa_l1] = "l1",
    [sa_r1] = "r1",
    [sa_l2] = "l2",
    [sa_r2] = "r2",
    [sa_l3] = "l3",
    [sa_r3] = "r3"
};

const char* ds3_get_axis_name(int index)
{
  return axis_name[index];
}

int ds3_get_button_index_from_name(const char* name)
{
  int i;
  for(i=0; i<AXIS_MAX; ++i)
  {
    if(!strcmp(axis_name[i], name))
    {
      return i;
    }
  }
  return -1;
}

int ds3_max_unsigned_axis_value[AXIS_MAX] =
{
  [sa_lstick_x] = MAX_AXIS_VALUE_8BITS,
  [sa_lstick_y] = MAX_AXIS_VALUE_8BITS,
  [sa_rstick_x] = MAX_AXIS_VALUE_8BITS,
  [sa_rstick_y] = MAX_AXIS_VALUE_8BITS,
  [sa_acc_x] = MAX_AXIS_VALUE_10BITS,
  [sa_acc_y] = MAX_AXIS_VALUE_10BITS,
  [sa_acc_z] = MAX_AXIS_VALUE_10BITS,
  [sa_gyro] = MAX_AXIS_VALUE_10BITS,
  [sa_select] = MAX_AXIS_VALUE_8BITS,
  [sa_start] = MAX_AXIS_VALUE_8BITS,
  [sa_ps] = MAX_AXIS_VALUE_8BITS,
  [sa_up] = MAX_AXIS_VALUE_8BITS,
  [sa_right] = MAX_AXIS_VALUE_8BITS,
  [sa_down] = MAX_AXIS_VALUE_8BITS,
  [sa_left] = MAX_AXIS_VALUE_8BITS,
  [sa_triangle] = MAX_AXIS_VALUE_8BITS,
  [sa_circle] = MAX_AXIS_VALUE_8BITS,
  [sa_cross] = MAX_AXIS_VALUE_8BITS,
  [sa_square] = MAX_AXIS_VALUE_8BITS,
  [sa_l1] = MAX_AXIS_VALUE_8BITS,
  [sa_r1] = MAX_AXIS_VALUE_8BITS,
  [sa_l2] = MAX_AXIS_VALUE_8BITS,
  [sa_r2] = MAX_AXIS_VALUE_8BITS,
  [sa_l3] = MAX_AXIS_VALUE_8BITS,
  [sa_r3] = MAX_AXIS_VALUE_8BITS,
};

unsigned int ds3_report_build(int axis[AXIS_MAX], s_report* report)
{
  s_report_ds3* ds3 = &report->value.ds3;

  ds3->report_id = 0x01;

  ds3->X = clamp(0, axis[sa_lstick_x] + 128, 255);
  ds3->Y = clamp(0, axis[sa_lstick_y] + 128, 255);
  ds3->Z = clamp(0, axis[sa_rstick_x] + 128, 255);
  ds3->Rz = clamp(0, axis[sa_rstick_y] + 128, 255);

  if(axis[sa_select])
  {
    ds3->buttons1 |= 0x01;
  }
  if(axis[sa_l3])
  {
    ds3->buttons1 |= 0x02;
  }
  if(axis[sa_r3])
  {
    ds3->buttons1 |= 0x04;
  }
  if(axis[sa_start])
  {
    ds3->buttons1 |= 0x08;
  }
  if(axis[sa_up])
  {
    ds3->buttons1 |= 0x10;
    ds3->up = axis[sa_up];
  }
  if(axis[sa_right])
  {
    ds3->buttons1 |= 0x20;
    ds3->right = axis[sa_right];
  }
  if(axis[sa_down])
  {
    ds3->buttons1 |= 0x40;
    ds3->down = axis[sa_down];
  }
  if(axis[sa_left])
  {
    ds3->buttons1 |= 0x80;
    ds3->left = axis[sa_left];
  }
  if(axis[sa_l2])
  {
    ds3->buttons2 |= 0x01;
    ds3->l2 = axis[sa_l2];
  }
  if(axis[sa_r2])
  {
    ds3->buttons2 |= 0x02;
    ds3->r2 = axis[sa_r2];
  }
  if(axis[sa_l1])
  {
    ds3->buttons2 |= 0x04;
    ds3->l1 = axis[sa_l1];
  }
  if(axis[sa_r1])
  {
    ds3->buttons2 |= 0x08;
    ds3->r1 = axis[sa_r1];
  }
  if(axis[sa_triangle])
  {
    ds3->buttons2 |= 0x10;
    ds3->triangle = axis[sa_triangle];
  }
  if(axis[sa_circle])
  {
    ds3->buttons2 |= 0x20;
    ds3->circle = axis[sa_circle];
  }
  if(axis[sa_cross])
  {
    ds3->buttons2 |= 0x40;
    ds3->cross = axis[sa_cross];
  }
  if(axis[sa_square])
  {
    ds3->buttons2 |= 0x80;
    ds3->square = axis[sa_square];
  }
  if(axis[sa_ps])
  {
    ds3->buttons3 |= 0x01;
  }

  unsigned short value;

  value = clamp(0, axis[sa_acc_x] + 512, 1023);
  ds3->acc_x[0] = value >> 8;
  ds3->acc_x[1] = value & 0xFF;
  value = clamp(0, axis[sa_acc_y] + 512, 1023);
  ds3->acc_y[0] = value >> 8;
  ds3->acc_y[1] = value & 0xFF;
  value = clamp(0, axis[sa_acc_z] + 400, 1023);
  ds3->acc_z[0] = value >> 8;
  ds3->acc_z[1] = value & 0xFF;
  value = clamp(0, axis[sa_gyro] + 512, 1023);
  ds3->gyro[0] = value >> 8;
  ds3->gyro[1] = value & 0xFF;

  ds3->unknown2[3] = 0x03;//not charging
  ds3->unknown2[4] = 0x05;//fully charged
  ds3->unknown2[5] = 0x10;//cable plugged in, no rumble

  ds3->unknown2[10] = 0x02;
  ds3->unknown2[11] = 0xff;
  ds3->unknown2[12] = 0x77;
  ds3->unknown2[13] = 0x01;
  ds3->unknown2[14] = 0x80;//no rumble

  return sizeof(report->value.ds3);
}

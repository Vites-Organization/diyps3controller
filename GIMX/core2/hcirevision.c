/*
 Copyright (c) 2011 Mathieu Laurendeau <mat.lau@laposte.net>
 License: GPLv3
 */

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  char command[64];
  int i;

  if(argc > 1 && sscanf(argv[1], "hci%d", &i) > 0)
  {
    sprintf(command, "hciconfig hci%d revision", i);

    system(command);
  }

  return 0;

}

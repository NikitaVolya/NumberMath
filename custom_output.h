#ifndef _CUSTOM_OUTPUT_H
#define _CUSTOM_OUTPUT_H

#include<termios.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

#include "vector2i.h"

#define BORDER_SYMBL "="
#define MESSAGE_BORDER 1

#define gotopos(pos) printf("\033[%d;%dH", (pos.y), (pos.x))

void repeat_print(const char *text, int n);

void set_input_mode(struct termios *old_tio);

void get_cursor_position(int *row, int *col);

void print_over(const char *text, vector2i pos);


#endif /* _CUSTOM_OUTPUT_H */
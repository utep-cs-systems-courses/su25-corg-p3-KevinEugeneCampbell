#ifndef POKEBALL_H
#define POKEBALL_H

#include "lcdutils.h"
#include "lcddraw.h"

typedef unsigned char u_char;
typedef unsigned int u_int;

void pokeball_draw_frame(u_char cx, u_char cy, u_char R, u_char theta_idx);
void pokeball_clear(u_char cx, u_char cy, u_char R, u_int bg);

#define PB_ANGLES 16

#endif

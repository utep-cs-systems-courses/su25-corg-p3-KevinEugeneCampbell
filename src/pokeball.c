#include "pokeball.h"
#include "lcddraw.h"
#include "lcdutils.h"
#include <stdint.h>

static const int16_t COS_Q8_8[PB_ANGLES] = {
  256, 237, 181,  98,   0,  -98, -181, -237,
  -256,-237,-181, -98,   0,   98,  181,  237
};

static const int16_t SIN_Q8_8[PB_ANGLES] = {
  0,  98, 181, 237, 256,  237, 181,   98,
  0, -98,-181,-237,-256, -237,-181,  -98
};



static inline int iabs_int(int x){ return x < 0 ? -x : x; }
static inline void pb_put(u_char x, u_char y, u_int color) { drawPixel(x, y, color); }
void pokeball_draw_frame(u_char cx, u_char cy, u_char R, u_char theta_idx) {
  if (theta_idx >= PB_ANGLES) theta_idx %= PB_ANGLES;
  
  const int16_t nx = COS_Q8_8[theta_idx];
  const int16_t ny = SIN_Q8_8[theta_idx];

  const int R2 = R * R;
  const int rBtnOuter = R/4;
  const int rBtnInner = (rBtnOuter > 2) ? (rBtnOuter-2) : rBtnOuter;
  const int bandHalfWidth = (R/6) * 256;

  for (int dy = -R; dy <= R; dy++) {
    for (int dx = -R; dx <= R; dx++) {
      int x = cx + dx;
      int y = cy + dy;
      int d2 = dx*dx + dy*dy;

      if (d2 > R2) continue;
      
      if ( (R - 1)*(R - 1) <= d2 ) {
	pb_put(x, y, COLOR_BLACK);
	continue;
      }

      int dBtn2 = dx*dx + dy*dy;

      if (dBtn2 <= rBtnOuter*rBtnOuter && dBtn2 >= rBtnInner*rBtnInner) {
	pb_put(x, y, COLOR_BLACK);
	continue;
      } else if (dBtn2 < rBtnInner*rBtnInner) {
	pb_put(x, y, COLOR_WHITE);
	continue;
      }

      int dot_q = (dx * nx + dy * ny);

      if (iabs_int(dot_q) <= bandHalfWidth){
	pb_put(x, y, COLOR_BLACK);
      } else if (dot_q > 0){
	pb_put(x, y, COLOR_RED);
      } else{
	pb_put(x, y, COLOR_WHITE);
      }
    }
  }
}
	

void pokeball_clear(u_char cx, u_char cy, u_char R, u_int bg) {
  fillRectangle(cx - R, cy - R, 2*R+1, 2*R+1, bg);
}

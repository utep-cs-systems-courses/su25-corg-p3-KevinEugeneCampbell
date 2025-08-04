#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "pokeball.h"
#include "pokemon_theme.h"
#include "buzzer.h"

#define LED_GREEN  BIT6

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8
#define SWITCHES (SW1|SW2|SW3|SW4)


#define BTN_SPIN_TOGGLE  SW1
#define BTN_RESET        SW2
#define BTN_THEME_TOGGLE SW3
#define BTN_THEME_STOP   SW4

short redrawScreen = 1;
static volatile char switches = 0;

static char switch_update_interrupt_sense(){
  char p2val = P2IN;
  P2IES |= (p2val & SWITCHES);
  P2IES &= (p2val | ~SWITCHES);
  return p2val;
}

static void switch_init(){
  P2REN |= SWITCHES;
  P2IE  |= SWITCHES;
  P2OUT |= SWITCHES;
  P2DIR &= ~SWITCHES;
  switch_update_interrupt_sense();
}

void switch_interrupt_handler(){
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
}



static volatile unsigned spinning = 0;
static volatile unsigned frame_div = 0;
static volatile unsigned led_div   = 0;
static volatile unsigned char theta_idx = 0;

static unsigned char CX = 64;
static unsigned char CY = 80;
static unsigned char R  = 28;

static void screen_init(){
  lcd_init();
  clearScreen(COLOR_BLUE);
  drawString5x7(2, 2,  (char*)"Gotta Catch 'Em All!", COLOR_WHITE, COLOR_BLUE);
  drawString5x7(2, 12, (char*)"SW1: Spin/Stop", COLOR_WHITE, COLOR_BLUE);
  drawString5x7(2, 22, (char*)"SW2: Reset", COLOR_WHITE, COLOR_BLUE);
  drawString5x7(2, 32, (char*)"SW3: Theme Song",  COLOR_WHITE, COLOR_BLUE);

  CX = screenWidth/2;
  CY = screenHeight/2;
  R  = (screenWidth < screenHeight ? screenWidth : screenHeight)/4;
  
  pokeball_draw_frame(CX, CY, R, theta_idx);
}

static void update_frame(){
  pokeball_clear(CX, CY, R, COLOR_BLUE);
  pokeball_draw_frame(CX, CY, R, theta_idx);
}

void wdt_c_handler(){
  static char prevSwitches = 0;
  pokemon_theme_wdt_tick();

  char now = switches;
  char pressed = (now & ~prevSwitches);
  prevSwitches = now;

  if (pressed & BTN_SPIN_TOGGLE) {
    spinning = !spinning;
  }
  if (pressed & BTN_RESET) {
    spinning = 0;
    theta_idx = 0;
    redrawScreen = 1;
  }
  if (pressed & BTN_THEME_TOGGLE) {
    if (!pokemon_theme_is_playing()) pokemon_theme_start();
    else pokemon_theme_stop();
  }
  if (pressed & BTN_THEME_STOP) {
    pokemon_theme_stop();
  }
  if (spinning) {
    if (++led_div >= 30) {
      led_div = 0;
      P1OUT ^= LED_GREEN;
    }
    if (++frame_div >= 16) {
      frame_div = 0;
      theta_idx = (theta_idx + 1) & 0x0F;
      redrawScreen = 1;
    }
  }
}

int main(){
  P1DIR |= LED_GREEN;
  P1OUT &= ~LED_GREEN;

  configureClocks();
  screen_init();
  switch_init();
  pokemon_theme_init();
  enableWDTInterrupts();
  or_sr(0x8);

  while (1) {
    if (redrawScreen) {
      redrawScreen = 0;
      update_frame();
    }
    or_sr(0x10);
  }
}

void __interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {
    P2IFG &= ~SWITCHES;
    switch_interrupt_handler();
  }
}

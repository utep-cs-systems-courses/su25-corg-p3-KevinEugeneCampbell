#include "pokemon_theme.h"
#include "libTimer.h"
#include "buzzer.h"

#define p_for(freq) ((unsigned)((freq) ? (2000000u/(unsigned)(freq)) : 0u))

enum {
  C4=262, E4=330, F4=349, G4=392, A4=440, Bb4=466
};

typedef struct { unsigned period; unsigned ticks; } Note;

static const Note song[] = {
  { p_for(A4), 72 }, {0,1}, { p_for(A4), 36 }, {0,1},
  { p_for(A4), 36 }, {0,1}, { p_for(A4), 72 }, {0,1},
  { p_for(A4), 72 }, { p_for(G4), 72 }, { p_for(E4), 72 }, { p_for(C4), 72 },
  { p_for(C4), 72 }, { p_for(A4), 36 }, {0,1}, { p_for(A4), 36 }, { p_for(G4), 36 },
  { p_for(F4), 36 }, { p_for(G4), 72 }, { p_for(F4), 72 }, { p_for(Bb4), 72 }, {0,1},
  { p_for(Bb4), 72 }, {0,1}, { p_for(Bb4), 72 }, { p_for(A4), 72 }, { p_for(G4), 72 },
  { p_for(F4), 72 }, {0,1}, { p_for(F4), 72 }, { p_for(A4), 72 }, {0,1}, { p_for(A4), 72 },
  { p_for(G4), 72 }, { p_for(F4), 72 }, { p_for(A4), 144 },
  { 0, 200 },
};

static const unsigned N_NOTES = sizeof(song)/sizeof(song[0]);
static volatile unsigned idx = 0;
static volatile unsigned tick = 0;
static volatile unsigned playing = 0;

void pokemon_theme_init(void) {
  buzzer_init();
  buzzer_set_period(0);
}
void pokemon_theme_start(void) {
  idx = 0; tick = 0; playing = 1;
  buzzer_set_period(song[0].period);
}

void pokemon_theme_stop(void) {
  playing = 0;
  buzzer_set_period(0);
}

bool pokemon_theme_is_playing(void){ return playing != 0; }

void pokemon_theme_wdt_tick(void) {
  if (!playing) return;
  if (++tick >= song[idx].ticks) {
    tick = 0;
    if (++idx >= N_NOTES) {
      pokemon_theme_stop();
    } else {
      buzzer_set_period(song[idx].period);
    }
  }
}

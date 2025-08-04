#ifndef POKEMON_THEME_H
#define POKEMON_THEME_H

#include <stdbool.h>

void pokemon_theme_init(void);
void pokemon_theme_start(void);
void pokemon_theme_stop(void);
bool pokemon_theme_is_playing(void);
void pokemon_theme_wdt_tick(void);

#endif

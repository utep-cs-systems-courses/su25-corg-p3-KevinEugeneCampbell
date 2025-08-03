#ifndef buzzer_included
#define buzzer_included

#define NOTE_G3  2551
#define NOTE_E3  3033
#define NOTE_C3  3823
#define NOTE_C4  1911
#define NOTE_E4  1517
#define NOTE_D4  1702
#define NOTE_FS3 2703
#define NOTE_REST 0

void buzzer_init();
void buzzer_set_period(short cycles);
void buzzer_off();
void play_song();


#endif // included

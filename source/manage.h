#ifndef MISC_C
#define MISC_C

#include "defines.h"
#include "ini.h"

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
// Please note that this pprogram dynamicly links the SDL_gfx library,
// which is licensed under the LGPL. While this software is not a
// derivative work, be aware that if you staticly link, or include the
// source of SDL_gfx, your work may fall under the category of
// derivative work, and may be subject to the LGPL.
#include "SDL/SDL_framerate.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>


// engine init/unload
int init_program(void);
void clean_up_game(void);

// surface management
SDL_Surface* load_image(char filename[]);
int load_images(void);
void free_images(void);

// audio management
int init_audio(void);
int load_sounds(void);
void free_sounds(void);

// audio playing
void play_sound(Sound_type sound_name, int sound_overwrite);

// quit/pause mechanism
void handle_events(int* play);

// other
void wait_for_keypress(void);


#endif
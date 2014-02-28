#ifndef RENDER_H
#define RENDER_H

#include "defines.h"
#include "SDL/SDL.h"
// Please note that this pprogram dynamicly links the SDL_gfx library,
// which is licensed under the LGPL. While this software is not a
// derivative work, be aware that if you staticly link, or include the
// source of SDL_gfx, your work may fall under the category of
// derivative work, and may be subject to the LGPL.
#include "SDL/SDL_rotozoom.h"
#include "SDL/SDL_gfxPrimitives.h"

enum screen_clip_result {NOT_OFFSCREEN, B_ONLY, C_ONLY, BCD};
typedef enum screen_clip_result Screen_clip_result;

void render_title_screen(Object choice_asteroid);

void render_game(Object ship, Object asteroids[], int num_asteroids, Object bullets[], int num_bullets, Object money[], int num_money, Object particles[], int num_particles);
void render_object(Object sprite);
void render_HUD(Object ship, int player);

Screen_clip_result object_off_screen(SDL_Rect obj);
void wrap_surface(int render_x, int render_y, SDL_Surface* surf);
void do_b(SDL_Surface* surf, SDL_Rect blit_rect, SDL_Rect pos_rect);
void do_c(SDL_Surface* surf, SDL_Rect blit_rect, SDL_Rect pos_rect);
void do_d(SDL_Surface* surf, SDL_Rect blit_rect, SDL_Rect pos_rect);

#endif
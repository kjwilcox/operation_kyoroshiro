// this is a file defining most of the generic values for the entire project
// any funtion using boolean values must include this header

#ifndef DEFINES_H
#define DEFINES_H

#include "struct.h"
#include "ini.h"
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
// Please note that this pprogram dynamicly links the SDL_gfx library,
// which is licensed under the LGPL. While this software is not a
// derivative work, be aware that if you staticly link, or include the
// source of SDL_gfx, your work may fall under the category of
// derivative work, and may be subject to the LGPL.
#include "SDL/SDL_framerate.h"

#define INI_NAME "settings.ini"
#define BACKUP_INI_NAME "settings.ini.bak"

// global variables for the external resources
dict settings;
Mix_Chunk* sound[10];
SDL_Surface* sprite_surface[10];
FPSmanager throttle;

#define TRUE 1
#define FALSE 0

// physics constants
// these will be wrapped up in the ship when I implement buying stuff
#define FRICTION_C .2
#define SPEED_LIMIT 20
#define ACCELERATION_C .7
#define ROTATION_C 4
#define GUN_COOLDOWN 14
#define BULLET_SPEED 11
#define BULLET_LIFE 80
#define MONEY_LIFE 400
#define ASTEROID_BREAK_FACTOR 1
#define MONEY_SPAWN_RATE 7

#define MAX_ASTEROIDS 700
#define MAX_BULLETS 100
#define MAX_MONEY 100
#define MAX_PARTICLES 1000

#endif
#ifndef LOGIC_H
#define LOGIC_H

#include "defines.h"
#include "kvector.h"
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include "helper.h"

// the ugly main logic functions
void ship_update(Object* ship, Object bullet[], int* num_bullets, Object particles[], int* num_particles, Uint8 keystate[]);
void asteroid_update(Object asteroid[], int num_asteroids);
void bullet_update(Object bullet[], int* num_bullets);
void money_update(Object money[], int* num_money);
void particle_update(Object particles[], int* num_particles);
void handle_bul_ast_collisions(Object asteroid[], int* num_asteroids, Object bullet[], int* num_bullets, Object money[], int* num_money);
void handle_bul_mon_collisions(Object money[], int* num_money, Object bullet[], int* num_bullets);
void handle_ship_mon_collisions(Object* ship, Object money[], int* num_money);
int ship_ast_collision(Object asteroid[], int num_asteroids, Object ship);
void check_ship_death(Object* ship);


#endif
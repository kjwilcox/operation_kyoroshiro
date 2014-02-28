#ifndef HELPER_H
#define HELPER_H

#include "defines.h"
#include "kvector.h"
#include "SDL/SDL.h"
#include <stdlib.h>
#include <math.h>

// helper functions
int collide(Object a, Object b);
Object add_bullet(Object ship);
Object add_money(Object asteroid);
Object add_particle(Object ship);

Object new_ship(void);
void empty_Object(Object* a);
Object random_asteroid(void);
void reset_ship(Object* ship);
void apply_velocity(Object* obj);
void wrap_position(Object* obj);
void wrap_angle(Object* obj);

// a macro because it's generic
#define MAX(a,b) (((a)>(b))?(a):(b))

#endif

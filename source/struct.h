
#ifndef STRUCT_H
#define STRUCT_H

#include "SDL/SDL.h"

struct x_y {
	double x;
	double y;
};
typedef struct x_y X_y;

struct vector {
	double heading;
	double magnitude;
};
typedef struct vector Vector;

struct ship_data {
	int invincible;
	int lives;
	int shield;
	int hit;
	int warp_cooldown;
	int gun_cooldown;
	int money;
	int score;
};
typedef struct ship_data Ship_data;

struct bullet_data {
	int ttl;
};
typedef struct bullet_data Bullet_data;

struct asteroid_data {
	double torque;
};
typedef struct asteroid_data Asteroid_data;

struct money_data {
	double torque;
	int ttl;
};
typedef struct money_data Money_data;

struct particle_data {
	int r;
	int g;
	int b;
	int ttl;
};
typedef struct particle_data Particle_data;

union unique_data {
	Ship_data s_data;
	Bullet_data b_data;
	Asteroid_data a_data;
	Money_data m_data;
	Particle_data p_data;
};
typedef union unique_data Unique_data;

enum obj_type {SCREEN, SHIP, BIG_ASTEROID, MEDIUM_ASTEROID, SMALL_ASTEROID, MONEY, BULLET, TITLE_SCREEN, PARTICLE};
typedef enum obj_type Obj_type;

enum sound_type {ASTEROID_BREAK, SHIP_DIE, SHIP_ENGINE, SHIP_HIT, MONEY_COLLECT, SHIP_SHOT, SHIP_WARP, MONEY_BREAK};
typedef enum sound_type Sound_type;

struct object {
	X_y pos;
	Vector velocity;
	double rotation;
	int size;
	Unique_data unique;
	Obj_type type;
};
typedef struct object Object;

struct key_set {
	Vector move;
	double heading;
};
typedef struct key_set Key_set;

#define EMPTY_UNIQUE {0}

#define EMPTY_XY {0, 0}
#define EMPTY_VECTOR {0, 0}
#define EMPTY_KEYSET {0, 0, 0}
#define EMPTY_OBJECT {EMPTY_XY, EMPTY_VECTOR, 0, 0, EMPTY_UNIQUE, MONEY}
#define EMPTY_SHIP {EMPTY_XY, EMPTY_VECTOR, 0, 0, EMPTY_UNIQUE, SHIP}
#define EMPTY_ASTEROID {EMPTY_XY, EMPTY_VECTOR, 0, 0, EMPTY_UNIQUE, BIG_ASTEROID}
#define EMPTY_BULLET {EMPTY_XY, EMPTY_VECTOR, 0, 0, EMPTY_UNIQUE, BULLET}
#define EMPTY_MONEY {EMPTY_XY, EMPTY_VECTOR, 0, 0, EMPTY_UNIQUE, MONEY}
#define EMPTY_PARTICLE {EMPTY_XY, EMPTY_VECTOR, 0, 0, EMPTY_UNIQUE, PARTICLE}


#endif
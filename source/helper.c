#include "helper.h"

int collide(Object a, Object b)
{
	double dist = 0;

	// calc distance between 2 points... good old pythagoras!
	// this could possibly be optimized by comparing the squares... maybe if nessecary
	dist = sqrt(pow(a.pos.x - b.pos.x, 2) + pow(a.pos.y - b.pos.y, 2));

	if (dist < (a.size + b.size))
		return TRUE;

	return FALSE;
}

Object add_bullet(Object ship)
{
	Object temp = EMPTY_BULLET;
	temp.pos = ship.pos;
	temp.velocity.heading = ship.rotation;
	temp.velocity.magnitude = BULLET_SPEED + ship.velocity.magnitude;
	temp.unique.b_data.ttl = BULLET_LIFE;
	temp.size = ini_val_i("bullet_size");
	temp.type = BULLET;
	temp.rotation = temp.velocity.heading;

	return temp;
}

Object add_money(Object asteroid)
{
	Object temp = EMPTY_MONEY;
	temp.pos = asteroid.pos;
	temp.velocity.heading = asteroid.rotation;
	temp.velocity.magnitude = 2 + rand() % 5;
	temp.unique.m_data.ttl = MONEY_LIFE;
	temp.size = ini_val_i("money_size");
	temp.unique.m_data.torque = 1 + rand() % 7;
	temp.type = MONEY;
	temp.rotation = temp.velocity.heading;

	return temp;
}

Object add_particle(Object ship)
{
	Object temp = EMPTY_PARTICLE;

	temp.pos = ship.pos;

	// add in a little randomness in the position
	temp.pos.x += ((rand() % 14) - 7 + 32);
	temp.pos.y += ((rand() % 14) - 7 + 32);

	// set velocity in opposite direction, slower
	temp.velocity = ship.velocity;
	temp.velocity.magnitude /= 4;
	temp.velocity.heading += (180 + ((rand() % 40) - 20));
	if (temp.velocity.heading > 360)
		temp.velocity.heading -= 360;

	// random colors. josh thought of HSV. look into it
	temp.unique.p_data.r = rand() % 256;
	temp.unique.p_data.g = rand() % 256;
	temp.unique.p_data.b = rand() % 256;

	// adjust lifetime so faster stuff burns out faster
	temp.unique.p_data.ttl = 15 + (rand() % 35) -
		((temp.velocity.magnitude > 0) ?
		(int) temp.velocity.magnitude : 0);

	return temp;
}


void empty_Object(Object* obj)
{
	obj->pos.x = 0;
	obj->pos.y = 0;
	obj->velocity.magnitude = 0;
	obj->velocity.heading = 0;
	obj->rotation = 0;
	obj->size = 0;

	switch (obj->type)
	{
	case BULLET:
		obj->unique.b_data.ttl = 0;
		break;
	case BIG_ASTEROID:
	case MEDIUM_ASTEROID:
	case SMALL_ASTEROID:
		obj->unique.a_data.torque = 0;
		break;
	case SHIP:
		obj->unique.s_data.gun_cooldown = 0;
		obj->unique.s_data.invincible = 0;
		obj->unique.s_data.lives = 0;
		obj->unique.s_data.shield = 0;
		obj->unique.s_data.warp_cooldown = 0;
		break;
	case MONEY:
		break;
	}
}

Object random_asteroid(void)
{
	// generate a random asteroid
	// warning: it might spawn on the player. too bad!
	Object temp = EMPTY_ASTEROID;
	temp.pos.x = rand() % ini_val_i("screen_width");
	temp.pos.y = rand() % ini_val_i("screen_height");
	temp.size = ini_val_i("big_asteroid_size");
	temp.velocity.heading = rand() % 360;
	temp.velocity.magnitude = 3 + rand() % 4;
	temp.type = 2;
	temp.unique.a_data.torque = 2 + rand() % 6;

	return temp;
}

Object new_ship(void)
{
	Object ship = EMPTY_SHIP;

	ship.size = ini_val_i("ship_size");
	ship.unique.s_data.lives = 2;
	reset_ship(&ship);

	return ship;
}

void reset_ship(Object* ship)
{
	ship->pos.x = ini_val_i("screen_width") / 2;;
	ship->pos.y = ini_val_i("screen_height") / 2;
	ship->velocity.heading = 90;
	ship->velocity.magnitude = 0;
	ship->rotation = 90;
	ship->unique.s_data.shield = 100; // magic number
	ship->unique.s_data.invincible = 120; // magic number
}

void apply_velocity(Object* obj)
{
	X_y temp;

	temp = Vector_to_X_y(obj->velocity);

	obj->pos.x += temp.x;
	obj->pos.y += -temp.y; // apply negative y velocity due to screen coordinates

	// wrap screen
	wrap_position(obj);
}

void wrap_position(Object* obj)
{
	// keep position in [0, screen_width) and [0, screen_height)
	obj->pos.x += (ini_val_i("screen_width") * (obj->pos.x < 0) - ini_val_i("screen_width")
		* (obj->pos.x >= ini_val_i("screen_width")));
	obj->pos.y += (ini_val_i("screen_height") * (obj->pos.y < 0) - ini_val_i("screen_height")
		* (obj->pos.y >= ini_val_i("screen_height")));
}

void wrap_angle(Object* obj)
{
	// keep angle in [0, 360)
	obj->rotation += (360 * (obj->rotation < 0) - 360 * (obj->rotation >= 360));
}

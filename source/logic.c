#include "defines.h"
#include "logic.h"

void handle_bul_ast_collisions(Object asteroid[], int* num_asteroids, Object bullet[], int* num_bullets, Object money[], int* num_money)
{
	int j = 0;
	int i = 0;

	for (j = 0; j < *num_asteroids; j += 1)
	{
		for (i = 0; i < *num_bullets; i += 1)
		{
			if (collide(asteroid[j], bullet[i]))
			{
				// play the sound
				Mix_PlayChannel(ASTEROID_BREAK, sound[ASTEROID_BREAK], 0);

				// break_asteroid(object asteroids[], int asteroid, int* num_asteroids);
				if (asteroid[j].type == SMALL_ASTEROID)
				{
					// destroy asteroid
					asteroid[j] = asteroid[*num_asteroids - 1];
					empty_Object(&asteroid[*num_asteroids - 1]);
					*num_asteroids -= 1;
					j -= 1; // look at new asteroid copied into this spot
				}
				else
				{
					if (asteroid[j].type == BIG_ASTEROID)
					{
						// pop out two medium ones
						asteroid[j].type = MEDIUM_ASTEROID;
						asteroid[j].size = ini_val_i("medium_asteroid_size");
					}
					else //(asteroid[j].type == MEDIUM_ASTEROID)
					{
						// pop out two small ones
						asteroid[j].type = SMALL_ASTEROID;
						asteroid[j].size = ini_val_i("small_asteroid_size");
					}

					// average bullet heading with asteroid heading
					asteroid[j].velocity.heading = (asteroid[j].velocity.heading + bullet[i].velocity.heading) / 2;

					asteroid[*num_asteroids] = asteroid[j];

					// rotate and randomly adjust magnitude
					asteroid[j].velocity.heading += 90;
					wrap_angle(&asteroid[j]);
					asteroid[j].velocity.magnitude += (rand() % (2 * ASTEROID_BREAK_FACTOR) - ASTEROID_BREAK_FACTOR);
					asteroid[j].velocity.magnitude = MAX(0, asteroid[j].velocity.magnitude);

					// rotate and randomly adjust magnitude
					asteroid[*num_asteroids].velocity.heading -= 90;
					wrap_angle(&asteroid[*num_asteroids]);
					asteroid[*num_asteroids].velocity.magnitude += (rand() % (2 * ASTEROID_BREAK_FACTOR) - ASTEROID_BREAK_FACTOR);
					asteroid[*num_asteroids].velocity.magnitude = MAX(0, asteroid[*num_asteroids].velocity.magnitude);

					*num_asteroids += 1;

					// spawn money sometimes
					if (rand() % MONEY_SPAWN_RATE == 0 && *num_money < MAX_MONEY) // magic number alert
					{
						money[*num_money] = add_money(asteroid[j]);
						*num_money += 1;
					}
				}

				//  destroy bullet
				bullet[i] = bullet[*num_bullets - 1];
				empty_Object(&bullet[*num_bullets - 1]);
				*num_bullets -= 1;

				break;
			}
		}
	}
}

void handle_bul_mon_collisions(Object money[], int* num_money, Object bullet[], int* num_bullets)
{
	int i = 0;
	int j = 0;

	for (j = 0; j < *num_money; j += 1)
	{
		for (i = 0; i < *num_bullets; i += 1)
		{
			if (collide(money[j], bullet[i]))
			{
				// play the sound
				Mix_PlayChannel(MONEY_BREAK, sound[MONEY_BREAK], 0);

				// destroy bullet
				bullet[i] = bullet[*num_bullets - 1];
				empty_Object(&bullet[*num_bullets - 1]);
				*num_bullets -= 1;

				// destroy money
				money[j] = money[*num_money - 1];
				empty_Object(&money[*num_money - 1]);
				*num_money -= 1;
				j -= 1; // look at new money moved to this slot

				break;
			}
		}
	}
}

void handle_ship_mon_collisions(Object* ship, Object money[], int* num_money)
{
	int i = 0;

	for (i = 0; i < *num_money; i += 1)
	{
		if (collide(*ship, money[i]))
		{
			ship->unique.s_data.money += 1;

			// play sound
			if (!Mix_Playing(MONEY_COLLECT))
				Mix_PlayChannel(MONEY_COLLECT, sound[MONEY_COLLECT], 0);

			// destroy money
			money[i] = money[*num_money - 1];
			empty_Object(&money[*num_money - 1]);
			*num_money -= 1;

			// move increment back so it will look at new money in same old space
			i -= 1;
		}
	}
}


void asteroid_update(Object asteroid[], int num_asteroids)
{
	int i = 0;

	for (i = 0; i < num_asteroids; i += 1)
	{
		apply_velocity(&asteroid[i]);
		asteroid[i].rotation += asteroid[i].unique.a_data.torque;
		wrap_angle(&asteroid[i]);
	}
}

void particle_update(Object particles[], int* num_particles)
{
	int i = 0;

	for (i = 0; i < *num_particles; i += 1)
	{
		apply_velocity(&particles[i]);

		// age particle
		particles[i].unique.p_data.ttl -= 1;

		// destroy particle if it's too old
		if (particles[i].unique.p_data.ttl == 0)
		{
			particles[i] = particles[*num_particles - 1];
			empty_Object(&particles[*num_particles - 1]);
			*num_particles -= 1;
			i -= 1; // look at new particle in this spot
		}
	}

}

void ship_update(Object* ship, Object bullet[], int* num_bullets, Object particles[], int* num_particles, Uint8 keystate[])
{
	Key_set input = EMPTY_KEYSET;
	int particle_num = 0;

	// create movement vectors based on input
	input.move.magnitude = ACCELERATION_C * (keystate[SDLK_UP]);
	input.heading = ROTATION_C * (keystate[SDLK_LEFT]) - ROTATION_C * (keystate[SDLK_RIGHT]);

	// adjust player rotation
	ship->rotation += input.heading;

	// keep in bounds
	wrap_angle(ship);

	// determine heading of movement vector 
	input.move.heading = ship->rotation;

	// adjust player vector
	ship->velocity = add_Vector(ship->velocity, input.move);

	// add friction by decreasing magnitude
	ship->velocity.magnitude -= FRICTION_C;

	// correct for negative vector...
	if (is_neg_Vector(ship->velocity))
		ship->velocity.magnitude = 0;

	// no speeding allowed
	if (ship->velocity.magnitude > SPEED_LIMIT)
		ship->velocity.magnitude = SPEED_LIMIT;

	// play movement sound
	if (keystate[SDLK_UP] && !Mix_Playing(SHIP_ENGINE))
		Mix_PlayChannel(SHIP_ENGINE, sound[SHIP_ENGINE], 0);

	// generate particles from engine
	if (keystate[SDLK_UP])
	{
		while (*num_particles < MAX_PARTICLES && particle_num < 5)
		{
			particles[*num_particles] = add_particle(*ship);

			*num_particles += 1;
			particle_num += 1;
		}

	}

	// lower warp cooldown
	if (ship->unique.s_data.warp_cooldown > 0)
	{
		ship->unique.s_data.warp_cooldown -= 1;
	}
	else if (keystate[SDLK_DOWN])
	{
		// move player to random coordinates and add cooldown
		ship->pos.x = rand() % ini_val_i("screen_width");
		ship->pos.y = rand() % ini_val_i("screen_height");
		ship->rotation = rand() % 360;
		ship->unique.s_data.warp_cooldown = 120; // MAGIC NUMBER

		// play warp sound
		if (!Mix_Playing(SHIP_WARP))
			Mix_PlayChannel(SHIP_WARP, sound[SHIP_WARP], 0);
	}

	// lower cooldown
	if (ship->unique.s_data.gun_cooldown > 0)
	{
		ship->unique.s_data.gun_cooldown -= 1;
	}
	else if (keystate[SDLK_SPACE] && *num_bullets < MAX_BULLETS)
	{
		// fire gun if there aren't too many bullets
		*num_bullets += 1;
		bullet[*num_bullets - 1] = add_bullet(*ship);
		ship->unique.s_data.gun_cooldown = GUN_COOLDOWN;

		// play gun sound
		Mix_PlayChannel(SHIP_SHOT, sound[SHIP_SHOT], 0);
	}

	// tick down invincibility
	if (ship->unique.s_data.invincible > 0)
		ship->unique.s_data.invincible -= 1;

	// apply damage
	if (ship->unique.s_data.hit > 0)
		ship->unique.s_data.shield -= ship->unique.s_data.hit;

	// play damage sound
	if (ship->unique.s_data.hit && !Mix_Playing(SHIP_HIT))
		Mix_PlayChannel(SHIP_HIT, sound[SHIP_HIT], 0);

	ship->unique.s_data.hit = 0;

	apply_velocity(ship);

}

void bullet_update(Object bullet[], int* num_bullets)
{
	int i = 0;

	for (i = 0; i < *num_bullets; i += 1)
	{
		// move bullets based on velocity
		apply_velocity(&bullet[i]);

		// age bullets
		bullet[i].unique.b_data.ttl -= 1;

		// destroy bullet if it's too old
		if (bullet[i].unique.b_data.ttl == 0)
		{
			bullet[i] = bullet[*num_bullets - 1];
			empty_Object(&bullet[*num_bullets - 1]);
			*num_bullets -= 1;
			i -= 1; // look at the new bullet occupying this spot
		}
	}
}

void money_update(Object money[], int* num_money)
{
	int i = 0;

	for (i = 0; i < *num_money; i += 1)
	{
		// move money based on velocity
		apply_velocity(&money[i]);

		// rotate money
		money[i].rotation += money[i].unique.m_data.torque;
		wrap_angle(&money[i]);

		// apply ttl
		money[i].unique.m_data.ttl -= 1;

		// destroy money if it's too old
		if (money[i].unique.m_data.ttl == 0)
		{
			money[i] = money[*num_money - 1];
			empty_Object(&money[*num_money - 1]);
			*num_money -= 1;
			i -= 1; // look at the new money occupying this spot
		}
	}
}

int ship_ast_collision(Object asteroid[], int num_asteroids, Object ship)
{
	int i = 0;
	int damage = 0;

	if (!ship.unique.s_data.invincible)
	{
		for (i = 0; i < num_asteroids; i += 1)
		{
			if (collide(asteroid[i], ship))
			{
				if (asteroid[i].type == BIG_ASTEROID)
					damage += 4;
				else if (asteroid[i].type == MEDIUM_ASTEROID)
					damage += 2;
				else
					damage += 1;
			}
		}
	}

	return damage;
}

void check_ship_death(Object* ship)
{
	if (ship->unique.s_data.shield < 0)
	{
		if (ship->unique.s_data.lives > 0)
			printf("You died. You have %d lives left.\n", ship->unique.s_data.lives);

		reset_ship(ship);

		ship->unique.s_data.lives -= 1;

		// stop sounds and play death sound, we want to sound important
		Mix_HaltChannel(-1);
		Mix_PlayChannel(SHIP_DIE, sound[SHIP_DIE], 0);
	}
}

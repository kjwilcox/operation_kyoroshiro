
#include "game.h"

int play_level(Object* ship, int level)
{
	int play = TRUE;

	Key_set input = EMPTY_KEYSET;

	Object asteroid[MAX_ASTEROIDS] = EMPTY_ASTEROID;
	Object money[MAX_MONEY] = EMPTY_MONEY;
	Object bullet[MAX_BULLETS] = EMPTY_BULLET;
	Object particles[MAX_PARTICLES] = EMPTY_PARTICLE;

	int num_asteroids = 0;
	int num_bullets = 0;
	int num_money = 0;
	int num_particles = 0;

	Uint8* keystate;

	// keeping num_asteroids under a certain value so the player can actually beat high levels
	for (num_asteroids = 0; num_asteroids < level && num_asteroids < ini_val_i("max_asteroids"); num_asteroids += 1)
		asteroid[num_asteroids] = random_asteroid(); // this is where we need to set up asteroids specially, right now they are random

	while (play && (num_asteroids > 0 || num_money > 0) && ship->unique.s_data.lives >= 0)
	{
		// get the user's keystate this frame
		keystate = SDL_GetKeyState(NULL);

		// move and rotate ship, fire bullets
		ship_update(ship, bullet, &num_bullets, particles, &num_particles, keystate);

		// move and rotate asteroids
		asteroid_update(asteroid, num_asteroids);

		// move and destroy bullets
		bullet_update(bullet, &num_bullets);

		// move and destroy money
		money_update(money, &num_money);

		// move and destroy particles
		particle_update(particles, &num_particles);

		// check for collisions between bullets and asteroids, will randomly create money
		handle_bul_ast_collisions(asteroid, &num_asteroids, bullet, &num_bullets, money, &num_money);

		// check for collisions between bullets and money
		handle_bul_mon_collisions(money, &num_money, bullet, &num_bullets);

		// handle collisions between money and ship
		handle_ship_mon_collisions(ship, money, &num_money);

		// handle collisions between asteroids and ship
		ship->unique.s_data.hit = ship_ast_collision(asteroid, num_asteroids, *ship);

		// render the screen
		render_game(*ship, asteroid, num_asteroids, bullet, num_bullets, money, num_money, particles, num_particles);

		// kill weak ship
		check_ship_death(ship);

		// handle system events throttle fps
		handle_events(&play);
	}

	if (ship->unique.s_data.lives < 0)
		play = FALSE;

	return play;
}
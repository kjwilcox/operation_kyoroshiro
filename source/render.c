#include "render.h"

void render_title_screen(Object choice_asteroid)
{
	extern SDL_Surface* sprite_surface[10];
	SDL_Rect offset;
	//int i = 0;

	// clear screen
	SDL_FillRect(sprite_surface[SCREEN], NULL, 0x000000);

	//// random sparkles!!
	//for (i = 0; i < 500; i += 1)
	//	pixelRGBA(sprite_surface[SCREEN],
	//	rand() % ini_val_i("screen_width"),
	//	rand() % ini_val_i("screen_height"),
	//	rand() % 256, rand() % 256, rand() % 256, 128);

	// graphics go here
	offset.x = (ini_val_i("screen_width") - 640) / 2;
	offset.y = (ini_val_i("screen_height") - 480) / 2;
	SDL_BlitSurface(sprite_surface[TITLE_SCREEN], NULL, sprite_surface[SCREEN], &offset);

	// show our cool asteroid
	render_object(choice_asteroid);

	SDL_Flip(sprite_surface[SCREEN]);
}


void render_object(Object sprite)
{
	extern SDL_Surface* sprite_surface[10];
	SDL_Rect offset;
	SDL_Surface* rotated_sprite = NULL;

	offset.x = (Sint16) sprite.pos.x;
	offset.y = (Sint16) sprite.pos.y;

	rotated_sprite = rotozoomSurface(sprite_surface[sprite.type], sprite.rotation, 1, 1);

	if (sprite.type == SHIP && sprite.unique.s_data.hit)
		filledCircleRGBA(rotated_sprite,
		rotated_sprite->w / 2,
		rotated_sprite->h / 2,
		32, 0, 127, 255, 128);

	if (ini_val_b("hitbox"))
		circleColor(rotated_sprite,
		rotated_sprite->w / 2,
		rotated_sprite->h / 2,
		sprite.size, 0x00FFFF);

	// compensate for the bigger image
	offset.x -= (rotated_sprite->w - 64) / 2;
	offset.y -= (rotated_sprite->h - 64) / 2;
	offset.h = rotated_sprite->h;
	offset.w = rotated_sprite->w;

	// wrap coordinates so they top-left is on screen
	if (offset.x < 0)
		offset.x += ini_val_i("screen_width");
	if (offset.y < 0)
		offset.y += ini_val_i("screen_height");

	// Blit the sprite, unless its and invicible ship
	if (sprite.type != SHIP || !sprite.unique.s_data.invincible || 1 != sprite.unique.s_data.invincible % 8)
		wrap_surface(offset.x, offset.y, rotated_sprite);

	// now that was a giant memory leak...
	SDL_FreeSurface(rotated_sprite); 
}

void render_HUD(Object ship, int player)
{
	// do nothing yet...
}


void wrap_surface(int render_x, int render_y, SDL_Surface* surf)
{
	extern SDL_Surface* sprite_surface[10];
	SDL_Rect blit_rect;
	SDL_Rect pos_rect;

	pos_rect.x = render_x;
	pos_rect.w = surf->w;
	pos_rect.y = render_y;
	pos_rect.h = surf->h;

	blit_rect = pos_rect;

	// blit normally
	// pos_rect recieves the rectangle that was blitted
	SDL_BlitSurface(surf, NULL, sprite_surface[SCREEN], &pos_rect);

	switch (object_off_screen(blit_rect))
	{
	case B_ONLY:
		do_b(surf, blit_rect, pos_rect);
		break;
	case C_ONLY:
		do_c(surf, blit_rect, pos_rect);
		break;
	case BCD:
		do_b(surf, blit_rect, pos_rect);
		do_c(surf, blit_rect, pos_rect);
		do_d(surf, blit_rect, pos_rect);
		break;
	}
}

void do_b(SDL_Surface* surf, SDL_Rect blit_rect, SDL_Rect pos_rect)
{
	extern SDL_Surface* sprite_surface[10];
	SDL_Rect src;
	SDL_Rect dst;

	src.x = pos_rect.w;
	src.w = surf->w - pos_rect.w;
	src.y = 0;
	src.h = pos_rect.h;

	dst.x = 0;
	dst.y = blit_rect.y;

	SDL_BlitSurface(surf, &src, sprite_surface[SCREEN], &dst);

}

void do_c(SDL_Surface* surf, SDL_Rect blit_rect, SDL_Rect pos_rect)
{
	extern SDL_Surface* sprite_surface[10];
	SDL_Rect src;
	SDL_Rect dst;

	src.x = 0;
	src.w = pos_rect.w;
	src.y = pos_rect.h;
	src.h = surf->h - pos_rect.h;

	dst.x = blit_rect.x;
	dst.y = 0;

	SDL_BlitSurface(surf, &src, sprite_surface[SCREEN], &dst);

}

void do_d(SDL_Surface* surf, SDL_Rect blit_rect, SDL_Rect pos_rect)
{
	extern SDL_Surface* sprite_surface[10];
	SDL_Rect src;
	SDL_Rect dst;

	src.x = pos_rect.w;
	src.w = surf->w - pos_rect.w;
	src.y = pos_rect.h;
	src.h = surf->h - pos_rect.h;

	dst.x = 0;
	dst.y = 0;

	SDL_BlitSurface(surf, &src, sprite_surface[SCREEN], &dst);

}

void render_game(Object ship, Object asteroids[], int num_asteroids, Object bullets[], int num_bullets, Object money[], int num_money, Object particles[], int num_particles)
{
	extern SDL_Surface* sprite_surface[10];
	int obj_num = 0;

	// clear screen
	SDL_FillRect(sprite_surface[SCREEN], NULL, 0x000000);

	// render particles
	for (obj_num = 0; obj_num < num_particles; obj_num += 1)
		pixelRGBA(sprite_surface[SCREEN], (Sint16) particles[obj_num].pos.x, (Sint16) particles[obj_num].pos.y,
		particles[obj_num].unique.p_data.r, particles[obj_num].unique.p_data.g, particles[obj_num].unique.p_data.b,
		255);

	// show asteroids
	for (obj_num = 0; obj_num < num_asteroids; obj_num += 1)
		render_object(asteroids[obj_num]);

	// show bullets
	for (obj_num = 0; obj_num < num_bullets; obj_num += 1)
		render_object(bullets[obj_num]);

	// show money
	for (obj_num = 0; obj_num < num_money; obj_num += 1)
		render_object(money[obj_num]);

	render_object(ship);

	// render the HUD
	render_HUD(ship, 1);

	// show the damn screen
	SDL_Flip(sprite_surface[SCREEN]);

}

Screen_clip_result object_off_screen(SDL_Rect obj)
{
	extern SDL_Surface* sprite_surface[10];
	SDL_Rect screen;

	screen = sprite_surface[SCREEN]->clip_rect;

	if (obj.x > 0 && obj.x < screen.w &&
		obj.x + obj.w > 0 && obj.x + obj.w < screen.w &&
		obj.y > 0 && obj.y < screen.h &&
		obj.y + obj.h > 0 && obj.y + obj.h < screen.h)
		return NOT_OFFSCREEN;
	else if (obj.y > 0 && obj.y < screen.h &&
		obj.y + obj.h > 0 && obj.y + obj.h < screen.h)
		return B_ONLY;
	else if (obj.x > 0 && obj.x < screen.w &&
		obj.x + obj.w > 0 && obj.x + obj.w < screen.w)
		return C_ONLY;
	else
		return BCD;

}


#include "menu.h"

Menu_option show_menu(void)
{
	Menu_option option = PLAY_GAME;
	int decided = FALSE;
	Object choice_asteroid = EMPTY_ASTEROID;

	// temp asteroids code
	choice_asteroid.pos.x = (ini_val_i("screen_width") - 640) / 2 + 1 ;
	choice_asteroid.pos.y = (ini_val_i("screen_height") - 480) / 2 + 190;
	choice_asteroid.unique.a_data.torque = 1 + rand() % 7;
	
	do {
		handle_title_screen_events(&option, &choice_asteroid, &decided);

		choice_asteroid.rotation += choice_asteroid.unique.a_data.torque;
		choice_asteroid.unique.a_data.torque += .05 * (1 - rand() % 3); // just for fun :O

		if (rand() % 1000 == 0)
			choice_asteroid.unique.a_data.torque = -choice_asteroid.unique.a_data.torque; // just for fun :D

		render_title_screen(choice_asteroid);
	} while (!decided);

	return option;
}

void handle_title_screen_events(Menu_option* current_choice, Object* choice_asteroid, int* decided)
{
	extern FPSmanager throttle;
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT ||
			(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
		{
			*current_choice = EXIT_PROGRAM;
			*decided = TRUE;
			break; // why even look at the rest of the events?
		}
		
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP && *current_choice > PLAY_GAME)
		{
			*current_choice -= 1;
			choice_asteroid->pos.y -= 52; // magic number for menu, needs to be changed if the title screen changes
		}

		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN && *current_choice < EXIT_PROGRAM)
		{
			*current_choice += 1;
			choice_asteroid->pos.y += 52; // magic number for menu, needs to be changed if the title screen changes
		}

		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
			*decided = TRUE;
	}

	SDL_framerateDelay(&throttle);
}

void execute_choice(Menu_option option)
{
	switch (option)
	{
	case PLAY_GAME:
		execute_play_game();
		break;
	case HIGH_SCORES:
		execute_high_scores();
		break;
	case MODIFY_OPTIONS:
		execute_modify_options();
		break;
	case VIEW_CREDITS:
		execute_view_credits();
		break;
	case EXIT_PROGRAM:
		execute_exit_program();
		break;
	}
}

void execute_play_game(void)
{
	Object ship;

	int play = TRUE;
	int level = 0;

	ship = new_ship();

	while (play)
	{
		level += 1;
		printf("Now entering level %d.\n", level);
		play = play_level(&ship, level);
	}

	printf("Your game ended on level %d.\n", level);
}

void execute_high_scores(void)
{
	printf("You picked 'high scores'.\nKyle has the high score, duh!\n");
}

void execute_modify_options(void)
{
	printf("You picked 'modify options'. But I won't let you.\n");
}

void execute_view_credits(void)
{
	printf("You picked 'view credits'...\nKyle wrote the game, Carl did the art.\n");
}

void execute_exit_program(void)
{
	// this function is blank, but i'll leave it here in case i can think of someting to add
}

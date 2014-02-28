#include "manage.h"

SDL_Surface* load_image(char filename[])
{ 
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;
	loadedImage = IMG_Load(filename);

	if (loadedImage != NULL)
	{
		optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
		SDL_FreeSurface(loadedImage); 
	}

	return optimizedImage; 
}

int load_images(void)
{
	extern SDL_Surface* sprite_surface[10];
	int img_num = 0;

	sprite_surface[SHIP] = load_image(ini_val_c("ship"));
	sprite_surface[BIG_ASTEROID] = load_image(ini_val_c("big_asteroid"));
	sprite_surface[MEDIUM_ASTEROID] = load_image(ini_val_c("medium_asteroid"));
	sprite_surface[SMALL_ASTEROID] = load_image(ini_val_c("small_asteroid"));
	sprite_surface[MONEY] = load_image(ini_val_c("money"));
	sprite_surface[BULLET] = load_image(ini_val_c("bullet"));
	sprite_surface[TITLE_SCREEN] = load_image(ini_val_c("title_screen"));


	// check for proper loading
	for (img_num = 1; img_num < ini_val_i("images") + 1; img_num += 1)
	{
		if (sprite_surface[img_num] == NULL)
		{
			printf("load_image: %s\n", IMG_GetError());
			return FALSE;
		}
	}

	return TRUE;
}


void free_images(void)
{
	extern SDL_Surface* sprite_surface[10];

	SDL_FreeSurface(sprite_surface[SHIP]);
	SDL_FreeSurface(sprite_surface[BIG_ASTEROID]);
	SDL_FreeSurface(sprite_surface[MEDIUM_ASTEROID]);
	SDL_FreeSurface(sprite_surface[SMALL_ASTEROID]);
	SDL_FreeSurface(sprite_surface[MONEY]);
	SDL_FreeSurface(sprite_surface[BULLET]);
	SDL_FreeSurface(sprite_surface[TITLE_SCREEN]);
}

int init_audio(void)
{
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		return FALSE;
	}

	Mix_AllocateChannels(ini_val_i("sounds"));

	return TRUE;
}

int load_sounds()
{
	int sound_num = 0;

	extern Mix_Chunk* sound[10];

	sound[ASTEROID_BREAK] = Mix_LoadWAV(ini_val_c("asteroid_break"));
	sound[SHIP_DIE] = Mix_LoadWAV(ini_val_c("ship_die"));
	sound[SHIP_ENGINE] = Mix_LoadWAV(ini_val_c("ship_engine"));
	sound[SHIP_HIT] = Mix_LoadWAV(ini_val_c("ship_hit"));
	sound[MONEY_COLLECT] = Mix_LoadWAV(ini_val_c("money_collect"));
	sound[SHIP_SHOT] = Mix_LoadWAV(ini_val_c("ship_shot"));
	sound[SHIP_WARP] = Mix_LoadWAV(ini_val_c("ship_warp"));
	sound[MONEY_BREAK] = Mix_LoadWAV(ini_val_c("money_break"));

	// check for proper loading
	for (sound_num = 0; sound_num < ini_val_i("sounds"); sound_num += 1)
	{
		if(!sound[sound_num]) {
			printf("Mix_LoadWAV: %s\n", Mix_GetError());
			return FALSE;
		}
	}

	return TRUE;
}

void play_sound(Sound_type sound_name, int sound_overwrite)
{
	extern Mix_Chunk* sound[10];

	if (!sound_overwrite || !Mix_Playing(sound_name))
		Mix_PlayChannel(sound_name, sound[sound_name], 0);
}

void free_sounds(void)
{
	extern Mix_Chunk* sound[10];

	// stop all sound
	Mix_HaltChannel(-1);

	// free all the sounds
	Mix_FreeChunk(sound[ASTEROID_BREAK]);
	Mix_FreeChunk(sound[SHIP_DIE]);
	Mix_FreeChunk(sound[SHIP_ENGINE]);
	Mix_FreeChunk(sound[SHIP_HIT]);
	Mix_FreeChunk(sound[MONEY_COLLECT]);
	Mix_FreeChunk(sound[SHIP_SHOT]);
	Mix_FreeChunk(sound[SHIP_WARP]);
}

void handle_events(int* play)
{
	extern FPSmanager throttle;
	SDL_Event event;
	int pause = FALSE;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT ||
			(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
		{
			*play = FALSE;
		}

		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_PAUSE)
		{
			pause = TRUE;
			while(pause)
			{
				SDL_framerateDelay(&throttle);
				while (SDL_PollEvent(&event))
				{
					if (event.type == SDL_QUIT ||
						(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
					{
						*play = FALSE;
						pause = FALSE;
					}

					if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_PAUSE)
					{
						pause = FALSE;
					}
				}
			}
		}
	}

	SDL_framerateDelay(&throttle);
}


int init_program(void)
{
	extern dict settings;
	extern SDL_Surface* sprite_surface[10];
	extern Mix_Chunk* sound[10];
	extern FPSmanager throttle;

	Uint32 flags = 0;


	int ini_load_fail;

	// load in settings.ini
	settings = load_ini(INI_NAME, &ini_load_fail);
	if (ini_load_fail)
	{
		printf("Unable to load the ini file.\n");

		printf("Attempting to load the backup...\n");
		settings = load_ini(BACKUP_INI_NAME, &ini_load_fail);
		if (ini_load_fail)
		{
			printf("Failed to load the backup, terminating...\n");
			return FALSE;
		}
		else
		{
			printf("Successfully loaded the backup ini file.\n");
		}
	}

	if (ini_val_b("print_ini"))
		ini_print();

	// seed RNG
	srand((unsigned int) time(NULL));

	// request that OS center the game window on screen
	SDL_putenv("SDL_VIDEO_WINDOW_POS");
	SDL_putenv("SDL_VIDEO_CENTERED=1");

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		printf("Failed to init SDL: %s\n", SDL_GetError());
		return FALSE;
	}

	// title bar title and icon
	SDL_WM_SetCaption("Operation Kyoroshiro", NULL);

	// disable nasty key-repeating
	SDL_EnableKeyRepeat(0, 0);

	// set FPS throttling to share some of the processor
	SDL_initFramerate(&throttle);
	SDL_setFramerate(&throttle, ini_val_i("fps_cap"));

	// check for too small values for screen size
	if (ini_val_i("screen_width") < 640 || ini_val_i("screen_height") < 480)
	{
		printf("Screen size too small. Minimum is 640 x 480.\n");
		return FALSE;
	}

	// try to initialize video
	flags |= (ini_val_b("software_surface")) ? SDL_SWSURFACE : SDL_HWSURFACE;
	flags |= (ini_val_b("fullscreen")) ? SDL_FULLSCREEN : 0x0;
	sprite_surface[SCREEN] = SDL_SetVideoMode(ini_val_i("screen_width"), ini_val_i("screen_height"), ini_val_i("screen_bpp"), flags);

	// catch failure of video intialization
	if (sprite_surface[SCREEN] == NULL)
	{
		printf("Failed to set up the screen: %s\n", SDL_GetError());
		return FALSE;
	}

	// load in image resources
	if (!load_images())
		return FALSE;

	// set up audio channels
	if (!init_audio())
		return FALSE;

	// load in sound resources
	if (!load_sounds())
		return FALSE;

	// everything worked! let's roll
	return TRUE;
}

void clean_up_game(void)
{
	free_images();
	free_sounds();
	Mix_CloseAudio();
	SDL_Quit();
}

void wait_for_keypress(void)
{
	printf("Press any key...");
	getchar();
	printf("\n");
}
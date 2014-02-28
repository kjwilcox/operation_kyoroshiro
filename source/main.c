
#include "defines.h"
#include "manage.h"
#include "menu.h"

int main(int argc, char* argv[])
{
	Menu_option option;

	if (!init_program())
	{
		// so they can see the error...
		wait_for_keypress(); 
		return 1;
	}

	do {
		option = show_menu();
		execute_choice(option);
	} while (option != EXIT_PROGRAM);

	clean_up_game();

	return 0;
}

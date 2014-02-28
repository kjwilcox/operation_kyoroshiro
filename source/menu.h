#ifndef MENU_H
#define MENU_H

#include "defines.h"
#include "game.h"
#include "helper.h"

enum menu_option {PLAY_GAME, HIGH_SCORES, MODIFY_OPTIONS, VIEW_CREDITS, EXIT_PROGRAM};
typedef enum menu_option Menu_option;

Menu_option show_menu(void);

void handle_title_screen_events(Menu_option* current_choice, Object* choice_asteroid, int* decided);

void execute_choice(Menu_option option);
void execute_play_game(void);
void execute_high_scores(void);
void execute_modify_options(void);
void execute_view_credits(void);
void execute_exit_program(void);

#endif
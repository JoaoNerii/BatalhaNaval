#ifndef IO_H
#define IO_H

#include <stdbool.h>
#include "game.h"

// Funções de entrada/saída
void io_print_menu();
void io_print_header();
void io_clear_screen();
void io_press_enter();
bool io_get_coordinates(const char *prompt, int *row, int *col, int max_rows, int max_cols);
int io_get_integer(const char *prompt, int min, int max);
char io_get_choice(const char *prompt, const char *valid_chars);
void io_get_string(const char *prompt, char *buffer, int max_len);
void io_place_ships_manual(Game *game, Player *player);
void io_place_ships_auto(Game *game, Player *player);
void io_show_game_over(Game *game);

// Sons simples
void play_hit_sound();
void play_miss_sound();
void play_sunk_sound();
void play_turn_sound(const char* player_name);

#endif
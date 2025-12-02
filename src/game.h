#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "fleet.h"

typedef struct {
    Board *board;       // MODIFICADO: Agora é ponteiro
    Board *shots;       // MODIFICADO: Agora é ponteiro
    Fleet *fleet;       // MODIFICADO: Agora é ponteiro
    char nickname[32];
    int total_shots;
    int hits;
} Player;

typedef struct {
    Player p1, p2;
    int current_player;
    int game_over;
    int board_size;
} Game;

// Protótipos das funções
Game* game_create();
void game_destroy(Game *game);
void game_init(Game *game, int board_size);
void game_switch_player(Game *game);
Player* game_get_current_player(Game *game);
Player* game_get_opponent(Game *game);
bool game_take_shot(Game *game, int row, int col);
bool game_is_over(Game *game);
void game_print_status(Game *game);
void game_print_boards(Game *game);
float game_get_accuracy(Player *player);

#endif
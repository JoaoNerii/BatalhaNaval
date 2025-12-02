#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "board.h"
#include "fleet.h"

Game* game_create() {
    Game *game = (Game*)malloc(sizeof(Game));
    if (!game) return NULL;
    
    memset(game, 0, sizeof(Game));
    
    // Inicializar ponteiros como NULL
    game->p1.board = NULL;
    game->p1.shots = NULL;
    game->p1.fleet = NULL;
    game->p2.board = NULL;
    game->p2.shots = NULL;
    game->p2.fleet = NULL;
    
    return game;
}

void game_destroy(Game *game) {
    if (game) {
        // Destruir estruturas do jogador 1
        if (game->p1.board) board_destroy(game->p1.board);
        if (game->p1.shots) board_destroy(game->p1.shots);
        if (game->p1.fleet) fleet_destroy(game->p1.fleet);
        
        // Destruir estruturas do jogador 2
        if (game->p2.board) board_destroy(game->p2.board);
        if (game->p2.shots) board_destroy(game->p2.shots);
        if (game->p2.fleet) fleet_destroy(game->p2.fleet);
        
        free(game);
    }
}

void game_init(Game *game, int board_size) {
    // Inicializar jogador 1
    if (game->p1.board) board_destroy(game->p1.board);
    if (game->p1.shots) board_destroy(game->p1.shots);
    if (game->p1.fleet) fleet_destroy(game->p1.fleet);
    
    game->p1.board = board_create(board_size, board_size);
    game->p1.shots = board_create(board_size, board_size);
    game->p1.fleet = fleet_create_default();
    game->p1.total_shots = 0;
    game->p1.hits = 0;
    game->p1.nickname[0] = '\0';
    
    // Inicializar jogador 2
    if (game->p2.board) board_destroy(game->p2.board);
    if (game->p2.shots) board_destroy(game->p2.shots);
    if (game->p2.fleet) fleet_destroy(game->p2.fleet);
    
    game->p2.board = board_create(board_size, board_size);
    game->p2.shots = board_create(board_size, board_size);
    game->p2.fleet = fleet_create_default();
    game->p2.total_shots = 0;
    game->p2.hits = 0;
    game->p2.nickname[0] = '\0';
    
    game->current_player = 0; // Jogador 1 começa
    game->game_over = 0;
    game->board_size = board_size;
}

void game_switch_player(Game *game) {
    game->current_player = 1 - game->current_player;
}

Player* game_get_current_player(Game *game) {
    return game->current_player == 0 ? &game->p1 : &game->p2;
}

Player* game_get_opponent(Game *game) {
    return game->current_player == 0 ? &game->p2 : &game->p1;
}

bool game_take_shot(Game *game, int row, int col) {
    Player *current = game_get_current_player(game);
    Player *opponent = game_get_opponent(game);
    
    bool hit;
    int ship_id;
    
    current->total_shots++;
    
    // Verificar se já atirou aqui
    Cell *shot_cell = board_get_cell(current->shots, row, col);
    if (!shot_cell || shot_cell->state != CELL_WATER) {
        return false;
    }
    
    // Marcar no tabuleiro do oponente
    if (board_mark_shot(opponent->board, row, col, &hit, &ship_id)) {
        // Marcar no tabuleiro de tiros do jogador atual
        Cell *cell = board_get_cell(current->shots, row, col);
        if (cell) {
            cell->state = hit ? CELL_HIT : CELL_MISS;
            cell->ship_id = ship_id;
        }
        
        if (hit) {
            current->hits++;
            // Atualizar hits no navio
            fleet_mark_hit(opponent->fleet, ship_id);
            
        }
        
        return true;
    }
    
    return false;
}

bool game_is_over(Game *game) {
    Player *p1 = &game->p1;
    Player *p2 = &game->p2;
    
    if (fleet_count_sunk_ships(p1->fleet) == p1->fleet->count) {
        game->game_over = 2; // Jogador 2 venceu
        return true;
    }
    
    if (fleet_count_sunk_ships(p2->fleet) == p2->fleet->count) {
        game->game_over = 1; // Jogador 1 venceu
        return true;
    }
    
    return false;
}

void game_print_status(Game *game) {
    Player *current = game_get_current_player(game);
    Player *opponent = game_get_opponent(game);
    
    printf("\n=== STATUS DO JOGO ===\n");
    printf("Jogador atual: %s\n", current->nickname);
    printf("Turno: %d\n", (current->total_shots + opponent->total_shots) / 2 + 1);
    printf("\n%s: %d acertos / %d tiros (%.1f%%)\n",
           current->nickname,
           current->hits,
           current->total_shots,
           game_get_accuracy(current));
    printf("%s: %d acertos / %d tiros (%.1f%%)\n",
           opponent->nickname,
           opponent->hits,
           opponent->total_shots,
           game_get_accuracy(opponent));
    
    // Navios afundados
    printf("\nNavios afundados:\n");
    printf("%s: %d/%d\n", 
           current->nickname,
           fleet_count_sunk_ships(current->fleet),
           current->fleet->count);
    printf("%s: %d/%d\n",
           opponent->nickname,
           fleet_count_sunk_ships(opponent->fleet),
           opponent->fleet->count);
}

void game_print_boards(Game *game) {
    Player *current = game_get_current_player(game);
    
    // Tabuleiro do jogador atual (seus navios)
    printf("\n=== SEU TABULEIRO ===\n");
    board_print_with_header(current->board, true, current->nickname);
    
    // Tabuleiro de tiros do jogador atual (visão do oponente)
    printf("\n=== SEUS TIROS ===\n");
    printf("Legenda: X = Acerto | . = Erro | ~ = Nao atirado\n");
    board_print_with_header(current->shots, false, "Mapa de tiros");
}

float game_get_accuracy(Player *player) {
    if (player->total_shots == 0) return 0.0f;
    return (float)player->hits / player->total_shots * 100.0f;
}

//Função para verificar se um navio foi afundado no último tiro
bool game_was_ship_sunk(Game *game, int row, int col) {
    Player *current = game_get_current_player(game);
    Player *opponent = game_get_opponent(game);
    
    Cell *cell = board_get_cell(current->shots, row, col);
    if (!cell || cell->state != CELL_HIT) {
        return false;
    }
    
    int ship_id = cell->ship_id;
    if (ship_id < 0) {
        return false;
    }
    
    return fleet_is_ship_sunk(opponent->fleet, ship_id);
}

//Função para obter o nome do navio em uma posição
const char* game_get_ship_name_at(Game *game, int row, int col) {
    Player *opponent = game_get_opponent(game);
    
    // Verificar no tabuleiro de tiros do jogador atual
    Player *current = game_get_current_player(game);
    Cell *cell = board_get_cell(current->shots, row, col);
    
    if (cell && cell->state == CELL_HIT && cell->ship_id >= 0) {
        if (cell->ship_id < opponent->fleet->count) {
            return opponent->fleet->ships[cell->ship_id].name;
        }
    }
    
    return "Navio";
}
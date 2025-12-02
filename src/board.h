#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

typedef enum { CELL_WATER, CELL_SHIP, CELL_HIT, CELL_MISS } CellState;

typedef struct {
    CellState state;
    int ship_id; // -1 se não houver navio
} Cell;

typedef struct {
    int rows, cols;
    Cell *cells; // malloc(rows * cols)
} Board;

// Funções para manipulação do tabuleiro
Board* board_create(int rows, int cols);
void board_destroy(Board *board);
void board_init(Board *board);
Cell* board_get_cell(Board *board, int row, int col);
bool board_place_ship(Board *board, int row, int col, int length, int orientation, int ship_id);
bool board_is_valid_position(Board *board, int row, int col);
bool board_is_empty_position(Board *board, int row, int col);
bool board_can_place_ship(Board *board, int row, int col, int length, int orientation);
void board_print(Board *board, bool show_ships);
void board_print_with_header(Board *board, bool show_ships, const char *title);
bool board_mark_shot(Board *board, int row, int col, bool *hit, int *ship_id);
int board_count_remaining_ships(Board *board, int *total_ships);

#endif
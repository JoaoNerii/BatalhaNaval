#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Board* board_create(int rows, int cols) {
    Board *board = (Board*)malloc(sizeof(Board));
    if (!board) return NULL;
    
    board->rows = rows;
    board->cols = cols;
    board->cells = (Cell*)malloc(rows * cols * sizeof(Cell));
    
    if (!board->cells) {
        free(board);
        return NULL;
    }
    
    board_init(board);
    return board;
}

void board_destroy(Board *board) {
    if (board) {
        if (board->cells) {
            free(board->cells);
        }
        free(board);
    }
}

void board_init(Board *board) {
    for (int i = 0; i < board->rows * board->cols; i++) {
        board->cells[i].state = CELL_WATER;
        board->cells[i].ship_id = -1;
    }
}

Cell* board_get_cell(Board *board, int row, int col) {
    if (!board_is_valid_position(board, row, col)) {
        return NULL;
    }
    return &board->cells[row * board->cols + col];
}

bool board_is_valid_position(Board *board, int row, int col) {
    return row >= 0 && row < board->rows && col >= 0 && col < board->cols;
}

bool board_is_empty_position(Board *board, int row, int col) {
    Cell *cell = board_get_cell(board, row, col);
    return cell && cell->state == CELL_WATER;
}

bool board_can_place_ship(Board *board, int row, int col, int length, int orientation) {
    if (orientation == 0) { // Horizontal
        if (col + length > board->cols) return false;
        for (int i = 0; i < length; i++) {
            if (!board_is_empty_position(board, row, col + i)) {
                return false;
            }
        }
    } else { // Vertical
        if (row + length > board->rows) return false;
        for (int i = 0; i < length; i++) {
            if (!board_is_empty_position(board, row + i, col)) {
                return false;
            }
        }
    }
    return true;
}

bool board_place_ship(Board *board, int row, int col, int length, int orientation, int ship_id) {
    if (!board_can_place_ship(board, row, col, length, orientation)) {
        return false;
    }
    
    if (orientation == 0) { // Horizontal
        for (int i = 0; i < length; i++) {
            Cell *cell = board_get_cell(board, row, col + i);
            cell->state = CELL_SHIP;
            cell->ship_id = ship_id;
        }
    } else { // Vertical
        for (int i = 0; i < length; i++) {
            Cell *cell = board_get_cell(board, row + i, col);
            cell->state = CELL_SHIP;
            cell->ship_id = ship_id;
        }
    }
    
    return true;
}

bool board_mark_shot(Board *board, int row, int col, bool *hit, int *ship_id) {
    Cell *cell = board_get_cell(board, row, col);
    if (!cell) return false;
    
    if (cell->state == CELL_HIT || cell->state == CELL_MISS) {
        return false; // Já atirou aqui
    }
    
    if (cell->state == CELL_SHIP) {
        cell->state = CELL_HIT;
        *hit = true;
        *ship_id = cell->ship_id;
        return true;
    } else {
        cell->state = CELL_MISS;
        *hit = false;
        *ship_id = -1;
        return true;
    }
}

int board_count_remaining_ships(Board *board, int *total_ships) {
    int remaining = 0;
    int total = 0;
    
    // Array para contar células de cada navio
    int max_ships = 10; // Número máximo de navios
    int *ship_cells = (int*)calloc(max_ships, sizeof(int));
    int *ship_hits = (int*)calloc(max_ships, sizeof(int));
    
    // Contar células e acertos por navio
    for (int i = 0; i < board->rows * board->cols; i++) {
        Cell cell = board->cells[i];
        if (cell.ship_id >= 0 && cell.ship_id < max_ships) {
            ship_cells[cell.ship_id]++;
            if (cell.state == CELL_HIT) {
                ship_hits[cell.ship_id]++;
            }
        }
    }
    
    // Verificar quais navios ainda estão inteiros
    for (int i = 0; i < max_ships; i++) {
        if (ship_cells[i] > 0) {
            total++;
            if (ship_hits[i] < ship_cells[i]) {
                remaining++;
            }
        }
    }
    
    if (total_ships) *total_ships = total;
    
    free(ship_cells);
    free(ship_hits);
    
    return remaining;
}

void board_print(Board *board, bool show_ships) {
    printf("  ");
    for (int col = 0; col < board->cols; col++) {
        printf("%c ", 'A' + col);
    }
    printf("\n");
    
    for (int row = 0; row < board->rows; row++) {
        printf("%2d ", row + 1);
        for (int col = 0; col < board->cols; col++) {
            Cell *cell = board_get_cell(board, row, col);
            char symbol;
            
            switch (cell->state) {
                case CELL_WATER:
                    symbol = show_ships && cell->ship_id >= 0 ? 'S' : '~';
                    break;
                case CELL_SHIP:
                    symbol = show_ships ? 'S' : '~';
                    break;
                case CELL_HIT:
                    symbol = 'X';
                    break;
                case CELL_MISS:
                    symbol = '.';
                    break;
                default:
                    symbol = '?';
            }
            
            printf("%c ", symbol);
        }
        printf("\n");
    }
}

void board_print_with_header(Board *board, bool show_ships, const char *title) {
    printf("\n%s\n", title);
    printf("Legenda: S = Navio | X = Acertado | ~ = Agua | . = Errou\n");
    board_print(board, show_ships);
}
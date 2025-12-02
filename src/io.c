#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "rnd.h"

// ==================== FUNÇÕES DE SOM SIMPLES ====================

void play_hit_sound() {
    printf("\n[ BOOOMMM! ] ACERTOU!\n");
}

void play_miss_sound() {
    printf("\n[ SPLAAASH! ] AGUA!\n");
}

void play_sunk_sound() {
    printf("\n[ GLUB GLUB! ]\n");
}

void play_turn_sound(const char* player_name) {
    printf("\n--- TURNO DE %s ---\n", player_name);
}

// ==================== FUNÇÕES ORIGINAIS ====================

void io_print_header() {
    io_clear_screen();
    printf("\n===== BATALHA NAVAL =====\n\n");
}

void io_clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void io_press_enter() {
    printf("\n(Pressione ENTER...)");
    while (getchar() != '\n');
    getchar();
}

void io_print_menu() {
    io_print_header();
    printf("1. Novo Jogo\n");
    printf("2. Configuracoes\n");
    printf("3. Instrucoes\n");
    printf("4. Sair\n");
    printf("\nEscolha: ");
}

void io_print_legend() {
    printf("\n--- LEGENDA ---\n");
    printf("S = Navio\n");
    printf("X = Acerto\n");
    printf(". = Erro\n");
    printf("~ = Agua\n");
}

bool io_get_coordinates(const char *prompt, int *row, int *col, int max_rows, int max_cols) {
    char input[32];
    printf("%s (ex: A1) ou 'Q' para sair: ", prompt);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return false;
    }
    
    // Remover newline
    input[strcspn(input, "\n")] = 0;
    
    // Verificar se o usuário quer sair
    if (strlen(input) == 1 && toupper(input[0]) == 'Q') {
        return false; // Retorna false para indicar que quer sair
    }
    
    if (strlen(input) < 2) {
        return false;
    }
    
    // Converter letra para coluna
    char col_char = toupper(input[0]);
    if (col_char < 'A' || col_char > 'Z') {
        return false;
    }
    
    *col = col_char - 'A';
    
    // Converter número para linha
    char *endptr;
    long row_val = strtol(input + 1, &endptr, 10);
    if (endptr == input + 1 || *endptr != '\0') {
        return false;
    }
    
    *row = (int)row_val - 1;
    
    // Verificar limites
    return *row >= 0 && *row < max_rows && *col >= 0 && *col < max_cols;
}
int io_get_integer(const char *prompt, int min, int max) {
    char input[32];
    
    while (1) {
        printf("\n%s (%d-%d): ", prompt, min, max);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            continue;
        }
        
        char *endptr;
        long val = strtol(input, &endptr, 10);
        
        if (endptr != input && *endptr == '\n' && val >= min && val <= max) {
            return (int)val;
        }
        
        printf("Invalido. Tente novamente.\n");
    }
}

char io_get_choice(const char *prompt, const char *valid_chars) {
    char input[32];
    
    while (1) {
        printf("\n%s: ", prompt);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            continue;
        }
        
        char choice = toupper(input[0]);
        
        for (int i = 0; valid_chars[i] != '\0'; i++) {
            if (choice == toupper(valid_chars[i])) {
                return choice;
            }
        }
        
        printf("Opcao invalida.\n");
    }
}

void io_get_string(const char *prompt, char *buffer, int max_len) {
    printf("\n%s: ", prompt);
    
    if (fgets(buffer, max_len, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    
    buffer[strcspn(buffer, "\n")] = 0;
}

void io_place_ships_manual(Game *game, Player *player) {
    (void)game;
    
    Fleet *fleet = player->fleet;
    Board *board = player->board;
    
    printf("\n--- Posicionar Navios: %s ---\n", player->nickname);
    
    for (int i = 0; i < fleet->count; i++) {
        Ship *ship = &fleet->ships[i];
        bool placed = false;
        
        while (!placed) {
            io_print_header();
            printf("Posicionar: %s (%d celulas)\n", ship->name, ship->length);
            board_print(board, true);
            
            int row, col;
            if (!io_get_coordinates("Coordenada inicial", &row, &col, 
                                   board->rows, board->cols)) {
                printf("Coordenada invalida!\n");
                io_press_enter();
                continue;
            }
            
            char orientation_choice = io_get_choice("Orientacao (H/V)?", "HV");
            int orientation = (orientation_choice == 'H') ? 0 : 1;
            
            if (board_place_ship(board, row, col, ship->length, orientation, i)) {
                ship->placed = 1;
                placed = true;
                printf("Navio posicionado!\n");
            } else {
                printf("Nao pode posicionar aqui!\n");
                io_press_enter();
            }
        }
    }
    
    printf("\nTodos navios posicionados!\n");
    board_print(board, true);
    io_press_enter();
}

void io_place_ships_auto(Game *game, Player *player) {
    (void)game;
    
    Fleet *fleet = player->fleet;
    Board *board = player->board;
    
    printf("\nPosicionando navios para %s...\n", player->nickname);
    
    for (int i = 0; i < fleet->count; i++) {
        Ship *ship = &fleet->ships[i];
        bool placed = false;
        int attempts = 0;
        
        while (!placed && attempts < 1000) {
            int row = rnd_range(0, board->rows - 1);
            int col = rnd_range(0, board->cols - 1);
            int orientation = rnd_orientation();
            
            if (board_place_ship(board, row, col, ship->length, orientation, i)) {
                ship->placed = 1;
                placed = true;
            }
            attempts++;
        }
    }
    
    printf("Pronto!\n");
}

void io_show_game_over(Game *game) {
    io_print_header();
    
    if (game->game_over == 1) {
        printf("\n*** VENCEDOR: %s! ***\n", game->p1.nickname);
    } else {
        printf("\n*** VENCEDOR: %s! ***\n", game->p2.nickname);
    }
    
    printf("\n--- ESTATISTICAS ---\n");
    printf("\n%s:\n", game->p1.nickname);
    printf("Tiros: %d | Acertos: %d | %.1f%%\n",
           game->p1.total_shots, game->p1.hits, game_get_accuracy(&game->p1));
    
    printf("\n%s:\n", game->p2.nickname);
    printf("Tiros: %d | Acertos: %d | %.1f%%\n",
           game->p2.total_shots, game->p2.hits, game_get_accuracy(&game->p2));
    
    printf("\n--- TABULEIROS FINAIS ---\n");
    
    printf("\n%s:\n", game->p1.nickname);
    board_print(game->p1.board, true);
    
    printf("\n%s:\n", game->p2.nickname);
    board_print(game->p2.board, true);
    
    printf("\n");
    io_press_enter();
}
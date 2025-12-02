#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "board.h"
#include "fleet.h"
#include "game.h"
#include "io.h"
#include "rnd.h"

void show_instructions() {
    io_print_header();
    printf("\n=== INSTRUCOES ===\n");
    printf("\nOBJETIVO:\n");
    printf("Afundar todos os navios do oponente antes que ele afunde os seus.\n");
    
    printf("\nNAVIOS:\n");
    printf("1 Porta-avioes (5 celulas)\n");
    printf("1 Encouracado (4 celulas)\n");
    printf("2 Cruzadores (3 celulas cada)\n");
    printf("2 Destroyers (2 celulas cada)\n");
    
    printf("\nCOORDENADAS:\n");
    printf("Digite a letra da coluna seguida do numero da linha.\n");
    printf("Exemplo: 'B5' atira na coluna B, linha 5.\n");
    
    printf("\nSIMBOLOS:\n");
    printf("S = Seu navio (visivel apenas no seu tabuleiro)\n");
    printf("X = Acerto (seu ou do oponente)\n");
    printf(". = Tiro na agua\n");
    printf("~ = Agua (nao atirado)\n");
    
    io_press_enter();
}

void game_settings(int *board_size, char *placement_mode) {
    io_print_header();
    printf("\n=== CONFIGURACOES ===\n");
    
    *board_size = io_get_integer("Tamanho do tabuleiro", 6, 26);
    *placement_mode = io_get_choice("Posicionamento (M)anual ou (A)utomatico", "MA");
    
    printf("\nConfiguracoes salvas:\n");
    printf("Tamanho do tabuleiro: %dx%d\n", *board_size, *board_size);
    printf("Posicionamento: %s\n", (*placement_mode == 'M') ? "Manual" : "Automatico");
    
    io_press_enter();
}

bool ask_to_quit() {
    char choice = io_get_choice("\nDeseja realmente sair para o menu? (S/N)", "SN");
    return (choice == 'S');
}

void start_new_game() {
    io_print_header();
    printf("\n=== NOVO JOGO ===\n");
    
    // Configurações padrão
    int board_size = 10;
    char placement_mode = 'A';
    
    // Perguntar se quer usar configurações personalizadas
    char choice = io_get_choice("Usar configuracoes padrao? (S)im ou (N)ao", "SN");
    
    if (choice == 'N') {
        game_settings(&board_size, &placement_mode);
    }
    
    // Criar e inicializar o jogo
    Game *game = game_create();
    if (!game) {
        printf("Erro ao criar o jogo!\n");
        io_press_enter();
        return;
    }
    
    game_init(game, board_size);
    
    // Obter nomes dos jogadores
    io_print_header();
    printf("\n=== CONFIGURACAO DOS JOGADORES ===\n");
    io_get_string("Nome do Jogador 1", game->p1.nickname, sizeof(game->p1.nickname));
    io_get_string("Nome do Jogador 2", game->p2.nickname, sizeof(game->p2.nickname)); 
    
    // Inicializar gerador de números aleatórios
    rnd_init();
    
    // Posicionar navios
    io_print_header();
    printf("\n=== FASE DE POSICIONAMENTO ===\n");
    
    if (placement_mode == 'M') {
        // Ambos manual
        io_place_ships_manual(game, &game->p1);
        io_place_ships_manual(game, &game->p2);
    } else if (placement_mode == 'A') {
        // Ambos automático
        io_place_ships_auto(game, &game->p1);
        io_place_ships_auto(game, &game->p2);
        
        printf("\nNavios posicionados automaticamente para ambos os jogadores.\n");
        io_press_enter();
    }
    
    // Loop principal do jogo
    while (!game_is_over(game)) {
        Player *current = game_get_current_player(game);
        
        io_print_header();
        game_print_status(game);
        
        // MOSTRAR TABULEIROS PERSONALIZADOS
        printf("\n=== TABULEIRO DE %s ===\n", current->nickname);
        board_print(current->board, true);
        
        printf("\n=== TIROS DE %s ===\n", current->nickname);
        board_print(current->shots, false);
        
        printf("\n=== TURNO DE %s ===\n", current->nickname);
        printf("(Digite 'Q' a qualquer momento para sair)\n");
        
        int row, col;
        bool valid_shot = false;
        
        while (!valid_shot) {
            char prompt[64];
            sprintf(prompt, "%s, digite coordenada para atirar", current->nickname);
            
            if (io_get_coordinates(prompt, &row, &col, game->board_size, game->board_size)) {
                if (game_take_shot(game, row, col)) {
                    valid_shot = true;
                    
                    // Mostrar resultado do tiro
                    Cell *shot_cell = board_get_cell(current->shots, row, col);
                    if (shot_cell) {
                        if (shot_cell->state == CELL_HIT) {
                            int ship_id = shot_cell->ship_id;
                            Player *opponent = game_get_opponent(game);
                            
                            if (ship_id >= 0 && ship_id < opponent->fleet->count) {
                                Ship *ship = &opponent->fleet->ships[ship_id];
                                
                                // Verificar se AFUNDOU ou apenas ACERTOU
                                if (fleet_is_ship_sunk(opponent->fleet, ship_id)) {
                                    // SOM PRIMEIRO
                                    play_sunk_sound();
                                    printf("\n*** ACERTOU e AFUNDOU um %s! ***\n", ship->name);
                                } else {
                                    // SOM PRIMEIRO
                                    play_hit_sound();
                                    printf("\n*** ACERTOU em um %s! ***\n", ship->name);
                                }
                            } else {
                                play_hit_sound();
                                printf("\n*** ACERTOU! ***\n");
                            }
                        } else {
                            // PARA AGUA: APENAS O SOM (que já mostra "AGUA")
                            play_miss_sound();
                        }
                    }
                } else {
                    printf("Voce ja atirou nesta posicao! Tente outra.\n");
                }
            } else {
                // Verificar se o usuário digitou 'Q' para sair
                printf("\n");
                if (ask_to_quit()) {
                    printf("\nSaindo para o menu principal...\n");
                    game_destroy(game);
                    io_press_enter();
                    return; // Retorna ao menu principal
                }
                printf("Coordenada invalida! Use formato como 'A1', 'B5', etc.\n");
            }
        }
        
        // Verificar se o jogo acabou
        if (game_is_over(game)) {
            break;
        }
        
        // Perguntar se quer sair entre turnos
        printf("\nDeseja continuar? (Pressione ENTER para continuar ou 'Q' para sair): ");
        char input[10];
        if (fgets(input, sizeof(input), stdin) != NULL) {
            if (toupper(input[0]) == 'Q') {
                if (ask_to_quit()) {
                    printf("\nSaindo para o menu principal...\n");
                    game_destroy(game);
                    io_press_enter();
                    return;
                }
            }
        }
        
        // Trocar jogador
        game_switch_player(game);
        
        printf("\n");
        io_press_enter();
    }
    
    // Mostrar tela de fim de jogo
    io_show_game_over(game);
    
    // Liberar memória
    game_destroy(game);
}

int main() {
    int running = 1;
    
    // Inicializar sistema de aleatoriedade
    rnd_init();
    
    while (running) {
        io_print_menu();
        
        char choice[32];
        if (fgets(choice, sizeof(choice), stdin) == NULL) {
            continue;
        }
        
        switch (choice[0]) {
            case '1':
                start_new_game();
                break;
                
            case '2': {
                int board_size;
                char placement_mode;
                game_settings(&board_size, &placement_mode);
                break;
            }
                
            case '3':
                show_instructions();
                break;
                
            case '4':
                printf("\nObrigado por jogar! Ate a proxima!\n");
                running = 0;
                break;
                
            default:
                printf("Opcao invalida! Tente novamente.\n");
                io_press_enter();
                break;
        }
    }
    
    return 0;
}
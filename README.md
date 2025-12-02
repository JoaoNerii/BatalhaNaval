# 🚢 Batalha Naval em C

*Projeto Acadêmico - CESAR School*  
Sistemas de Informação - PIF 2025.2

## 📋 Equipe de Desenvolvimento

| Nome | Papel Principal |

| *Danilo de Melo Duarte* | Arquitetura e Lógica do Jogo |
| *Pedro Pessoa Bastos* | Sistema de Entrada/Saída e Interface |
| *João Neri* | Gerenciamento de Memória e Validações |

## 🎯 Objetivo do Projeto

Implementar o clássico jogo *Batalha Naval* utilizando exclusivamente as bibliotecas básicas da linguagem C, demonstrando domínio dos seguintes conceitos fundamentais:

- *Structs* para modelagem de dados
- *Ponteiros* e manipulação de endereços de memória
- *Alocação Dinâmica* (malloc, realloc, free)
- *Fluxos de Controle* e lógica de programação
- *Modularização* e organização de código

## 🚀 Funcionalidades Implementadas

### ✅ Características Principais
- *Tabuleiro configurável* (6x6 a 26x26) com padrão 10x10
- *Frota completa* conforme especificação:
  - 1 Porta-aviões (5 células)
  - 1 Encouraçado (4 células)
  - 2 Cruzadores (3 células cada)
  - 2 Destroyers (2 células cada)
- *Modos de posicionamento*:
  - Manual (jogador posiciona cada navio)
  - Automático (posicionamento aleatório)
- *Sistema de turnos* alternados entre jogadores
- *Validação rigorosa* de todas as entradas
- *Interface CLI* completa e intuitiva
- *Sons simulados* com texto ASCII para feedback auditivo
- *Opção de saída* durante o jogo (voltar ao menu)

### ✅ Estruturas de Dados
```c
typedef enum { CELL_WATER, CELL_SHIP, CELL_HIT, CELL_MISS } CellState;
typedef struct { CellState state; int ship_id; } Cell;
typedef struct { int rows, cols; Cell *cells; } Board;
typedef struct { char name[20]; int length; int hits; int placed; } Ship;
typedef struct { Ship *ships; int count; } Fleet;
typedef struct { Board *board; Board *shots; Fleet *fleet; char nickname[32]; int total_shots; int hits; } Player;
typedef struct { Player p1, p2; int current_player; int game_over; int board_size; } Game

## 📂 Estrutura do Projeto
|
| `board.h / board.c` | Representação do tabuleiro       |
| `fleet.h / fleet.c` | Gestão da frota de navios        |
| `game.h / game.c`   | Lógica e fluxo principal do jogo |
| `io.h / io.c`       | Entrada e saída (interface CLI)  |
| `rnd.h / rnd.c`     | Geração de números aleatórios    |
| `main.c`            | Ponto de entrada do programa     |

## ▶️ Como Executar o Projeto

### 1. Abra o terminal (PowerShell, CMD ou outro)

Navegue até a pasta do projeto:

```bash
cd caminho/ate/batalha-naval-pif-2025/src

### 2. Compile o Jogo

gcc main.c board.c fleet.c game.c io.c rnd.c -o batalha_naval.exe

### 3. Execute o Jogo

./batalha_naval.exe
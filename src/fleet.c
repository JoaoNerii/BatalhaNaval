#include "fleet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Fleet* fleet_create_default() {
    Fleet *fleet = (Fleet*)malloc(sizeof(Fleet));
    if (!fleet) return NULL;
    
    fleet->count = 6; // 1+1+2+2 navios
    fleet->ships = (Ship*)malloc(fleet->count * sizeof(Ship));
    
    if (!fleet->ships) {
        free(fleet);
        return NULL;
    }
    
    // Porta-aviões (id 0)
    strcpy(fleet->ships[0].name, "Porta-avioes");
    fleet->ships[0].length = 5;
    fleet->ships[0].hits = 0;
    fleet->ships[0].placed = 0;
    
    // Encouraçado (id 1)
    strcpy(fleet->ships[1].name, "Encouracado");
    fleet->ships[1].length = 4;
    fleet->ships[1].hits = 0;
    fleet->ships[1].placed = 0;
    
    // Cruzadores (ids 2-3)
    strcpy(fleet->ships[2].name, "Cruzador");
    fleet->ships[2].length = 3;
    fleet->ships[2].hits = 0;
    fleet->ships[2].placed = 0;
    
    strcpy(fleet->ships[3].name, "Cruzador");
    fleet->ships[3].length = 3;
    fleet->ships[3].hits = 0;
    fleet->ships[3].placed = 0;
    
    // Destroyers (ids 4-5)
    strcpy(fleet->ships[4].name, "Destroyer");
    fleet->ships[4].length = 2;
    fleet->ships[4].hits = 0;
    fleet->ships[4].placed = 0;
    
    strcpy(fleet->ships[5].name, "Destroyer");
    fleet->ships[5].length = 2;
    fleet->ships[5].hits = 0;
    fleet->ships[5].placed = 0;
    
    return fleet;
}

void fleet_destroy(Fleet *fleet) {
    if (fleet) {
        if (fleet->ships) {
            free(fleet->ships);
        }
        free(fleet);
    }
}

Ship* fleet_get_ship(Fleet *fleet, int ship_id) {
    if (!fleet || ship_id < 0 || ship_id >= fleet->count) {
        return NULL;
    }
    return &fleet->ships[ship_id];
}

bool fleet_is_ship_sunk(Fleet *fleet, int ship_id) {
    Ship *ship = fleet_get_ship(fleet, ship_id);
    return ship && ship->hits >= ship->length;
}

int fleet_count_sunk_ships(Fleet *fleet) {
    int count = 0;
    for (int i = 0; i < fleet->count; i++) {
        if (fleet_is_ship_sunk(fleet, i)) {
            count++;
        }
    }
    return count;
}

int fleet_count_total_ships(Fleet *fleet) {
    return fleet->count;
}

void fleet_mark_hit(Fleet *fleet, int ship_id) {
    Ship *ship = fleet_get_ship(fleet, ship_id);
    if (ship && ship->hits < ship->length) {
        ship->hits++;
    }
}

void fleet_reset(Fleet *fleet) {
    for (int i = 0; i < fleet->count; i++) {
        fleet->ships[i].hits = 0;
        fleet->ships[i].placed = 0;
    }
}

void fleet_print_status(Fleet *fleet) {
    printf("\n=== Status da Frota ===\n");
    for (int i = 0; i < fleet->count; i++) {
        Ship *ship = &fleet->ships[i];
        char sunk_status = fleet_is_ship_sunk(fleet, i) ? 'S' : 'N';
        printf("%-15s: %d/%d %c\n", 
               ship->name, 
               ship->hits, 
               ship->length,
               sunk_status);
    }
}
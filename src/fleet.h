#ifndef FLEET_H
#define FLEET_H

#include "board.h"

typedef enum { ORIENT_H, ORIENT_V } Orientation;

typedef struct {
    char name[20];
    int length;
    int hits;
    int placed;
} Ship;

typedef struct {
    Ship *ships;
    int count;
} Fleet;

// Funções para frota
Fleet* fleet_create_default();
void fleet_destroy(Fleet *fleet);
Ship* fleet_get_ship(Fleet *fleet, int ship_id);
bool fleet_is_ship_sunk(Fleet *fleet, int ship_id);
int fleet_count_sunk_ships(Fleet *fleet);
int fleet_count_total_ships(Fleet *fleet);
void fleet_mark_hit(Fleet *fleet, int ship_id);
void fleet_reset(Fleet *fleet);
void fleet_print_status(Fleet *fleet);

#endif
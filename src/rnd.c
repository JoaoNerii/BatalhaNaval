#include "rnd.h"
#include <stdlib.h>
#include <time.h>

void rnd_init() {
    srand((unsigned int)time(NULL));
}

int rnd_range(int min, int max) {
    return rand() % (max - min + 1) + min;
}

bool rnd_bool() {
    return rand() % 2 == 0;
}

int rnd_orientation() {
    return rand() % 2; // 0 = Horizontal, 1 = Vertical
}
#ifndef RND_H
#define RND_H

#include <stdbool.h>

// Funções de aleatoriedade
void rnd_init();
int rnd_range(int min, int max);
bool rnd_bool();
int rnd_orientation();

#endif
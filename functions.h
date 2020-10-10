#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "image.h"
#include <vector>

int manhattan_dist(Image x, Image y, int rows, int columns);

void lsh(int k, int L, int N, double R, std::vector<Image> q);

#endif
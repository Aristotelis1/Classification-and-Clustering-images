#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "image.h"
#include <vector>

int manhattan_dist(vector<unsigned char> image1, vector<unsigned char> image2 ,int dimension);

void lsh(int k, int L, int N, double R, std::vector<Image> q);

#endif
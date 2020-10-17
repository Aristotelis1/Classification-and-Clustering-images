#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "image.h"
#include <vector>

int manhattan_dist(vector<unsigned char> image1, vector<unsigned char> image2 ,int dimension);

void lsh(int k, int L, int N, double R, std::vector<Image> q);

int get_image_pos (vector<unsigned char>image);
int change_endianess (int big_end);
int set_int_from_bytes (unsigned char byte1, unsigned char byte2, unsigned char byte3);
void get_bytes_from_int (int source, unsigned char &byte1, unsigned char &byte2, unsigned char &byte3);
int modifyBit(int n, int p, int b);
int key_shake(int key, int p);
int change_neighbor(int key, int p, int k);

#endif
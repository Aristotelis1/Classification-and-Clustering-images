#ifndef HASH_FUNCTIONS_H
#define HASH_FUNCTIONS_H


#include <iostream> 
#include <vector>
#include <cstdlib>
#include <ctime>
#include <math.h>

#include "functions.h"

#define MAX 255

using namespace std;



class Hash_Function {
    private:
        unsigned long int dimension, k, m, mod;   //mod is M on algorithm
        vector<double> s_h;
        int w;

    public:
        Hash_Function(int dim, vector<double> s, unsigned int k_out);
        unsigned long int get_hash_key(vector<unsigned char> image, int w);
    
};


double get_mean_range(int samples, vector<vector<unsigned char>> images);
int get_w(int r);
unsigned long int mod_exp(unsigned long int base, unsigned long int exp, unsigned long int mod);
int get_a(unsigned char x, double s, int w);
unsigned long int concatenate_h(vector<Hash_Function>hfs, vector<unsigned char> image, int w);

#endif
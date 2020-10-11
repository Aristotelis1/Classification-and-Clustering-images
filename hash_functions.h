#include <iostream> 
#include <vector>
#include <cstdlib>
#include <ctime>
#include <math.h>


using namespace std;

class Hash_Function {
    private:
        unsigned int dimension, k, m, mod;   //mod is M on algorithm
        vector<double> s_h;

    public:
        Hash_Function(int dim, vector<double>s, int k_out);
        int get_hash_key(vector<unsigned char> image, int w);
    
};


double get_mean_range(int samples, vector<vector<unsigned char>> images);
int get_w(int r);
int mod_exp(unsigned long int base, unsigned long int exp, unsigned long int mod);
int get_a(unsigned char x, double s, int w);
int concatenate_h();
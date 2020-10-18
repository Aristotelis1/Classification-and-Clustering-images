#include "hash_functions.h"




Hash_Function::Hash_Function(int dim, vector<double>s, unsigned int k_out){
    int r;
    dimension=dim;
    k=k_out;
//    m=pow(2,31)-5;
    int temp=32/k;
    mod=pow(2,temp);
    m=mod/4 + 1;
    s_h.resize(dimension);
    int s_size=s.size();
    for (int i=0; i<dimension ; i++){
        r=rand()%s_size;
        s_h[i]=s[r];    //random pick of s
    }

}

unsigned long int Hash_Function::get_hash_key(vector<unsigned char> image, int w){
    unsigned long int h=0, sum, a, x;     //x represents m^d
    for (int i=0; i<dimension; i++){
        sum=0;
        a=get_a(image[i], s_h[i], w);
        if (a!=0){
            x=mod_exp(m, dimension-(i+1), mod);
//            a=a%mod;
//            sum=(x*a)%mod;
            sum=a*x;
            if (sum%m < 0)
                sum=sum%mod + mod;
            h= (h+sum)%mod;
        }
    }
//    cout<<h<<endl;

    return h;
    

}


double get_mean_range(int samples, vector<vector<unsigned char>> images){
    int i, image1, image2, dimension, j;

    dimension=images[1].size()-3;
    int number_of_images=images.size(), mindist;
    long long int nearest_mean=0;
    for (i=0; i<samples ; i++){
        mindist=999999;
        for (j=i+1; j<(number_of_images) ; j++){
            if (manhattan_dist(images[i], images[j], dimension)<mindist)
                mindist=manhattan_dist(images[i], images[j], dimension);
        }
        nearest_mean+=mindist;

    }
     return nearest_mean/samples;


}

int get_w(int r){
    return r*4;

//    return 80000;

}


unsigned long int mod_exp(unsigned long int base, unsigned long int exp, unsigned long int mod){
    unsigned long int res=1;
    base=base % mod;
    if (base == 0)
        return 0;

    while (exp > 0)  
    {  
        // If exp is odd, multiply base with result  
        if (exp & 1)  
            res = (res*base) % mod;  
  
        // exp must be even now  
        exp = exp>>1; // y = y/2  
        base = (base*base) % mod;  
    }  

    return res;
}


int get_a(unsigned char x, double s, int w){
    if (x-s<0)
        return -1;
    else
        return 0;

}

unsigned long int concatenate_h(vector<Hash_Function>hfs, vector<unsigned char> image, int w){
    int k, bits_shift;
    unsigned long int g=0, temp;

    k=hfs.size();
    bits_shift=32/k;
    for (int i=0; i<k ; i++){
        temp= hfs[i].get_hash_key(image, w);
        temp= temp<< (i*bits_shift);
        g = g | temp;
    }

    return g;

}
#include "hash_functions.h"
#include "functions.h"


Hash_Function::Hash_Function(int dim, vector<double>s, int k_out){
    srand(time(NULL));

    dimension=dim;
    k=k_out;
    m=pow(2,32)-5;
    mod=pow(2,32/k);
    s_h.resize(dimension);
    int s_size=s.size();
    for (int i=0; i<dimension ; i++){
        s_h[i]=s[rand()%s_size];    //random pick of s
    }

    cout<<dimension<<" k:"<<k<<" m:"<<m<<" mod:"<<mod<<endl;
}

int Hash_Function::get_hash_key(vector<unsigned char> image, int w){
    unsigned int h=0, sum, a, x;     //x represents m^d
    for (int i=0; i<dimension; i++){
        sum=0;
        a=get_a(image[i], s_h[i], w);
        x=mod_exp(m, dimension-(i+1), mod);
        a=a%mod;
        sum=(x+a)%mod;
        h+= sum%mod;
    }
}


double get_mean_range(int samples, vector<vector<unsigned char>> images){
    srand(time(NULL));
    int i, image1, image2, dimension;
    double distance, mean=0.0;

    dimension=images[1].size();
    for (i=0; i<samples ; i++){
        image1=rand()%images.size();
        image2=rand()%images.size();
//        cout<< "image1: " << image1 << "--image2: "<< image2;
        distance=manhattan_dist(images[image1], images[image2], dimension);
//        cout<<"-->man dist: "<< distance;
        mean+=(distance/(double)samples);
//        cout<<"--> mean dist: " << mean << endl;
    }
    return mean;
}

int get_w(int r){
    return r*4;
}


int mod_exp(unsigned long int base, unsigned long int exp, unsigned long int mod){
    int res=1;
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
    return (abs((double)x-s)/w);
}

int concatenate_h(){



}
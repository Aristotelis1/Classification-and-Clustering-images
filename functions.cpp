#include <cmath>
#include <vector>
#include <string>
#include <iostream>

#include "functions.h"

using namespace std;


// Manhattan distance
int manhattan_dist(vector<unsigned char> image1, vector<unsigned char> image2 ,int dimension) 
{ 
    int sum = 0; 
  
    // for each point, finding distance to 
    // rest of the point
    // string x_s = x.get_image();
    // string y_s = y.get_image(); 
    // vector<unsigned char> x_s = x.get_image();
    // vector<unsigned char> y_s = y.get_image();
    // for (int i = 0; i < rows; i++) 
    //     for (int j = i + 1; j < columns; j++) 
    //         sum += (abs(x_s[i] - y_s[i]));

    // return sum; 

    for (int i=0; i < dimension; i++){
        sum += (abs(image1[i]-image2[i]));
    }

    return sum;

}




// LSH function
void lsh(int k, int L, int N, double R, vector<Image> q)
{
    // NN

}


int get_image_pos (vector<unsigned char>image){
    int size=image.size();
    int pos;
    unsigned char byte3=image[size-1];
    unsigned char byte2=image[size-2];
    unsigned char byte1=image[size-3];
    pos= set_int_from_bytes(byte1, byte2, byte3);
    return pos+1;
}

int change_endianess (int big_end) 
{
    unsigned char byte1, byte2, byte3, byte4;
    int little_end;
    
    byte1 = big_end & 255;
    byte2 = (big_end >> 8) & 255;
    byte3 = (big_end >> 16) & 255;
    byte4 = (big_end >> 24) & 255;
    little_end = ((int)byte1 << 24) + ((int)byte2 << 16) + ((int)byte3 << 8) + byte4;


    return little_end;
}

int set_int_from_bytes (unsigned char byte1, unsigned char byte2, unsigned char byte3){
    int b1 = byte1, b2 = byte2, b3 = byte3, r;
    b1 = (b1 << 16);
    b2 = (b2 << 8);
    r=(b1 | b2 | b3);
    return r;
}

void get_bytes_from_int (int source, unsigned char &byte1, unsigned char &byte2, unsigned char &byte3){
    byte1 = 0xFF;
    byte2 = 0xFF;
    byte3 = 0xFF;
    byte3 = (byte3 & source);
    source = (source>>8);   
    byte2 = (byte2 & source);
    source = (source>>8);   
    byte1 = (byte1 & source);
}

int modifyBit(int n, int p, int b){ 
    int mask = 1 << p; 
    return (n & ~mask) | ((b << p) & mask); 
}

int key_shake(int key, int p){
    int temp=key;
    temp=temp>>(p-1);
    if ((temp%2)==0)
        key=modifyBit(key, p-1, 1);
    else
        key=modifyBit(key, p-1, 0);
    return key;
}

int change_neighbor(int key, int p, int k){
    int z, i;

    for (z=k; z>0; z--){
        for(i=1; i<=z; i++){
            if(p==i){
                key=key_shake(key, i);
                return key;
            }
        }
        key=key_shake(key, z);
        p = p-z;
    }

    return key;
}

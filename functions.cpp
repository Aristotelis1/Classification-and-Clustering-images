#include <cmath>
#include <vector>
#include <string>

#include "image.h"

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
#include <cmath>

#include "structs.h"


// Manhattan distance
int manhattan_dist(Image x, Image y,int rows,int  columns) 
{ 
    int sum = 0; 
  
    // for each point, finding distance to 
    // rest of the point
    char** x_matrix = x.get_matrix();
    char** y_matrix = y.get_matrix(); 
    for (int i = 0; i < rows; i++) 
        for (int j = i + 1; j < columns; j++) 
            sum += (abs(x_matrix[i][j] - y_matrix[i][j]));

    return sum; 
} 


// LSH function
void lsh(int k, int L, int N, double R)
{

}

#include "structs.h"

Image :: Image()
{

}

char ** Image :: get_matrix()
{
    return matrix;
}

char Image :: get_pixel(int i, int j)
{
    return matrix[i][j];
}
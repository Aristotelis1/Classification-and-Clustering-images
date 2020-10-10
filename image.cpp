
#include "image.h"
#include <string>
#include <vector>

using namespace std;

Image :: Image(vector<unsigned char> m)
{
    image = m;
}

vector<unsigned char> Image :: get_image()
{
    return image;
}

unsigned char Image :: get_pixel(int i)
{
    return image[i];
}

Image :: ~Image()
{

}
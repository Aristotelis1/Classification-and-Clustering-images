
#include "image.h"
#include <string>

Image :: Image(std::string m)
{
    image = m;
}

std::string Image :: get_image()
{
    return image;
}

char Image :: get_pixel(int i)
{
    return image[i];
}
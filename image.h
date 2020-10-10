#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <list>


class Image {
    private:
        std::string image;
    public:
        Image(std::string);

        ~Image();

        std::string get_image();

        char get_pixel(int i);

        
};



#endif
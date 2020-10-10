#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <list>
#include <vector>

//typedef std::basic_string<unsigned char> ustring;

using namespace std;


class Image {
    private:
        //string image;
        vector<unsigned char> image;
    public:
        //Image(string);
        Image(vector<unsigned char>);

        ~Image();

        vector<unsigned char> get_image();

        unsigned char get_pixel(int i);

        
};



#endif
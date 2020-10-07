#ifndef STRUCTS_H
#define STRUCTS_H


class Image {
    private:
        char** matrix;
    public:
        Image();

        char ** get_matrix();

        char get_pixel(int i , int j);
};

#endif
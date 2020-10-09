#ifndef STRUCTS_H
#define STRUCTS_H


// https://www.geeksforgeeks.org/unordered_map-in-cpp-stl/

class Image {
    private:
        char** matrix;
    public:
        Image(char **);

        char ** get_matrix();

        char get_pixel(int i , int j);
};

#endif
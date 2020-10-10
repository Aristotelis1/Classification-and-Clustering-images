#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>
#include <list>
#include "image.h"

using namespace std;



class Hash {
    private:
        int bucket;
        list< vector<unsigned char> > *table;
    public:
        Hash(int v); // Constructor

        ~Hash();

        // insert
        void insertItem(vector<unsigned char> i);

        void displayHash();

};


class ImageTable {
    private:
        string image;
};


#endif
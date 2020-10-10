#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>
#include <list>
#include "image.h"

using namespace std;



class Hash {
    private:
        int bucket;
        list<Image*> *table;
    public:
        Hash(int v); // Constructor

        // insert
        void insertItem(Image i);

        void displayHash();

};


class ImageTable {
    private:
        string image;
};


#endif
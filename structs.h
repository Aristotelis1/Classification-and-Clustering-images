#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>
#include <list>
#include <array>
#include "image.h"

using namespace std;

// class image
// {
//     private:
//         vector<unsigned char>* img;
//     public:
//         image(vector<unsigned char> i);

// };

class Hash_list
{
    private:
        list<vector<unsigned char>*> list_of_images;
    public:
        Hash_list();
        void add_image(vector<unsigned char> &i);
        void display_list();
        void clear();
        void searchByKey();
};



class Hash {
    private:
        int bucket;
        // array< list< vector<unsigned char>*>> *table;
        Hash_list* hash_table;
        
    public:
        Hash(int v); // Constructor

        ~Hash();

        // insert
        void insertItem(vector<unsigned char> &i);

        void displayHash();

        void searchByKey(int index);

};


class ImageTable {
    private:
        string image;
};


#endif
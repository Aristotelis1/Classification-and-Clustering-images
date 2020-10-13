#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>
#include <list>
#include <array>
#include <queue>
#include "image.h"

using namespace std;



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
        void insertItem(vector<unsigned char> &i, int key);

        void displayHash();

        void searchByKey(int index);

};


class image
{
    private:
        vector<unsigned char>* img;
        int distance;

    public:
        image(int dist, vector<unsigned char> i);
        int get_distance();
};

bool operator<(image& i1, image& i2) 
{ 
    return i1.get_distance() < i2.get_distance(); 
} 

class PQ{
    private:
        priority_queue<image> pq;
        int maxDistance;

    public:
        PQ(list<vector<unsigned char>*> bucket);
        PQ(vector<vector<unsigned char>> images);

};

#endif
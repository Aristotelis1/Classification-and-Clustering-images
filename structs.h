#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>
#include <list>
#include <array>
#include <queue>
#include "image.h"
#include "functions.h"

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


    public:
        int distance;
        image(int dist, vector<unsigned char> &i);
        int get_distance();
};

bool operator<(const image& i1, const image& i2) 
{ 
    return i1.distance < i2.distance; 
} 

class PQ{
    private:
        priority_queue<image> pq;
        int maxDistance;
        list<vector<unsigned char>*> *bucket;
        vector<vector<unsigned char>> *images;

    public:
        PQ(list<vector<unsigned char>*> b, vector<unsigned char> query, int N);
        PQ(vector<vector<unsigned char>> imgs, vector<unsigned char> query, int N);

        image getNN(); //gurnaei th prwth thesi tou pq
        image get_k_NN(); //gurnaei th thesi k

};

#endif
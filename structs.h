#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>
#include <list>
#include <array>
#include <queue>
#include <fstream>
#include "image.h"
#include "functions.h"
#include "hash_functions.h"

using namespace std;



class Hash_list
{
    private:
        vector<vector<unsigned char>> list_of_images;
    public:
        Hash_list();
        void add_image(vector<unsigned char> &i);
        void display_list();
        void clear();
        void searchByKey();
        vector<vector<unsigned char>> get_list_of_images();

};



class Hash {
    private:
        int bucket, w;
        unsigned long int testg;
        vector<Hash_Function> hfunctions;
        Hash_list* hash_table;
        
    public:
        Hash(int number_of_images, vector<vector<unsigned char>> images, int dimension, int k, vector<double>s, int in_w); // Constructor

        ~Hash();

        // insert
        void insertItem(vector<unsigned char> i);

        void displayHash();

        void searchByKey(int index);

        int calculate_g(vector<unsigned char> img);

        vector<vector<unsigned char>> get_list_of_images(int key);


};

class Cube {
    private:
        int vertices, w;
        unsigned long int testg;
        vector<Hash_Function> hfunctions;
        Hash_list* cube_vertex;
        
    public:
//        Hash(int number_of_images, vector<vector<unsigned char>> images, int dimension, int k, vector<double>s, int in_w); // Constructor
        Cube(vector<vector<unsigned char>> images, int dimension, int k, vector<double>s, int in_w);
        int calculate_fh(int key);
        int calculate_vector_key(vector<unsigned char>images);
        void insertItem(vector<unsigned char> i);
        void displayCube();
        vector<vector<unsigned char>> get_list_of_images(int key);

};

class image
{
    private:
        vector<unsigned char> img;


    public:
        int distance;
        image(int dist, vector<unsigned char> &i);
        image(const image &im2);
        int get_distance();
        vector<unsigned char> get_image();
        friend bool operator<(const image& i1, const image& i2) 
        { 
            return i1.distance < i2.distance; 
        } 
};


class PQ{
    private:
        priority_queue<image> pq;
        priority_queue<int, vector<int>, greater<int>> range;
        int maxDistance;
        // list<vector<unsigned char>*> *bucket;
        // vector<vector<unsigned char>> *images;

    public:
        PQ(vector<unsigned char> query, int N,vector<Hash> hash_tables);
        PQ(vector<vector<unsigned char>> imgs, vector<unsigned char> query, int N);
        //PQ(vector<unsigned char> query,vector<Hash> hash_tables,int r);
        PQ(vector<unsigned char> query, int N, Cube hypercube, int M, int probes, int k);
        void displayN();
        image getNN(); //gurnaei th prwth thesi tou pq
        image get_k_NN(); //gurnaei th thesi k
        priority_queue<image> get_pq();
        void range_search(int r, vector<Hash> hash_tables, vector<unsigned char> query);
        void cube_range_search(int r, Cube hypercube, vector<unsigned char> query, int probes, int k, int M);

        void displayRange(ofstream & out);

};

void display_prqueues(PQ pq_lsh, PQ pq_exhaust, string type, ofstream& out);

#endif
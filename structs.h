#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>
#include <list>
#include <array>
#include <queue>
#include <fstream>
#include "helping_functions.h"
#include "hash_functions.h"

using namespace std;



class Hash_list
{
    private:
        vector<vector<unsigned char> > list_of_images;
        vector<unsigned long int> labels;
    public:
        Hash_list();
        void add_image(vector<unsigned char> &i);
        void add_imagel(vector<unsigned char> &i, unsigned long int label);
        void display_list();
        void clear();
        void searchByKey();
        vector<vector<unsigned char> > get_list_of_images();
        vector<unsigned long int>get_labels();

};



class Hash {
    private:
        int bucket, w;
        unsigned long int testg;
        vector<Hash_Function> hfunctions;
        //vector<Hash_list> hash_table;
        Hash_list* hash_table;
        
    public:
        Hash(int number_of_images, vector<vector<unsigned char> > images, int dimension, int k, vector<double>s, int in_w); // Constructor
        ~Hash();
        void insertItem(vector<unsigned char> i);
        void displayHash();
        void searchByKey(int index);
        int calculate_g(vector<unsigned char> img);
        vector<vector<unsigned char> > get_list_of_images(int key);
        vector<unsigned long int> get_labels(int key);
        vector<Hash_Function> get_hfs();
        int get_w();

};

class Cointoss{
    private:

    public:
        int f, h;
        Cointoss(int inf, int inh);

};

class Cube {
    private:
        int vertices, w;
        unsigned long int testg;
        vector<Hash_Function> hfunctions;
        Hash_list* cube_vertex;
        vector<Cointoss> flist;
        
    public:
//        Hash(int number_of_images, vector<vector<unsigned char>> images, int dimension, int k, vector<double>s, int in_w); // Constructor
        Cube(vector<vector<unsigned char> > images, int dimension, int k, vector<double>s, int in_w);
        int calculate_fh(int key);
        int calculate_vector_key(vector<unsigned char>images);
        void insertItem(vector<unsigned char> i);
        void displayCube();
        vector<vector<unsigned char> > get_list_of_images(int key);

};

class image
{
    private:
        vector<unsigned char> img;
        unsigned long int label;
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
        priority_queue<int, vector<int>, greater<int> > range;
        int maxDistance;


    public:
        PQ(vector<unsigned char> query, int N,vector<Hash> hash_tables, vector<unsigned long int> query_keys,vector<unsigned long int> query_labels);
        PQ(vector<vector<unsigned char> > imgs, vector<unsigned char> query, int N);
        PQ(vector<unsigned char> query, int N, Cube hypercube, int M, int probes, int k, int key, vector<int> nb);
        void displayN();
        image getNN(); //gurnaei th prwth thesi tou pq
        image get_k_NN(); //gurnaei th thesi k
        priority_queue<image> get_pq();
        void range_search(int r, vector<Hash> hash_tables, vector<unsigned char> query);
//        vector<vector<unsigned char> > lsh_images_in_range(int r, vector<Hash> hash_tables, vector<unsigned char> query);
        void cube_range_search(int r, Cube hypercube, vector<unsigned char> query, int probes, int k, int M);
//        vector<vector<unsigned char> > cube_images_in_range(int r, Cube hypercube, vector<unsigned char> query, int probes, int k, int M);
        void displayRange(ofstream & out);

};

void display_prqueues(PQ pq_lsh, PQ pq_exhaust, string type, ofstream& out);
vector<vector<unsigned char> > lsh_images_in_range(int r, vector<Hash> hash_tables, vector<unsigned char> query);
vector<vector<unsigned char> > cube_images_in_range(int r, Cube hypercube, vector<unsigned char> query, int probes, int k, int M);


#endif
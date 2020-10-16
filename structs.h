#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>
#include <list>
#include <array>
#include <queue>
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

        void displayN();
        image getNN(); //gurnaei th prwth thesi tou pq
        image get_k_NN(); //gurnaei th thesi k
        priority_queue<image> get_pq();
        void range_search(int r, vector<Hash> hash_tables, vector<unsigned char> query);
        void displayRange();

};


class Point{
    private:
        int clusterId,dimensions;
        vector<unsigned char> image;
    public:
        Point(vector<unsigned char> image,int dimensions);
        int get_dimensions();
        unsigned char getPixel(int position);
        void set_cluster(int id);

};

class Cluster{
    private:
        vector<Point> images;
        vector<unsigned char> center;
        int clusterId;
    public:
        Cluster(int clusterId,Point centroid);
        vector<unsigned char> get_center();
        void calculate_center();
};

class KMeans{
    private:
        int K,number_of_points,dimensions;
        vector<Cluster> clusters;
    public:
        KMeans(int K);
        int get_nearest_cluster_id(Point point);
        void run(vector<Point>& all_points);


};

void display_prqueues(PQ pq_lsh, PQ pq_exhaust);

#endif
#ifndef STRUCTS_CLUSTER_H
#define STRUCTS_CLUSTER_H

#include <string>
#include <list>
#include <array>
#include <queue>

#include "structs.h"

using namespace std;


class Point{
    private:
        int clusterId,dimensions;
        vector<unsigned char> image;
    public:
        Point(vector<unsigned char> image,int dimensions);
        int get_dimensions();
        unsigned char getPixel(int position);
        vector<unsigned char> get_image();
        void set_cluster(int id);
        int get_cluster();

};

class Cluster{
    private:
        vector<Point> images;
        vector<unsigned char> center;
        int clusterId;
    public:
        Cluster(int clusterId,Point centroid);
        vector<unsigned char> get_center();
        void calculate_center(int dimensions); // den exei ulopohithei
        int get_clusterId();
        void add_image(Point point);
        bool remove(Point point);
        int get_size();
        int calculate_average_distance();
};

class KMeans{
    private:
        int K,number_of_points,dimensions; // K is number of clusters
        vector<Cluster> clusters;
    public:
        KMeans(int K);
        int get_nearest_cluster_id(Point point);
        void run(vector<Point>& all_points);
        int get_nearest_cluster(Point point);
        void silhouette();
        void lsh(vector<Point>& all_points,vector<Hash> hash_tables);


};

#endif
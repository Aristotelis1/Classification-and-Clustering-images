#ifndef STRUCTS_CLUSTER_H
#define STRUCTS_CLUSTER_H

#include <string>
#include <list>
#include <array>
#include <queue>

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
        void calculate_center(); // den exei ulopohithei
        int get_clusterId();
        void add_image(Point point);
};

class KMeans{
    private:
        int K,number_of_points,dimensions;
        vector<Cluster> clusters;
    public:
        KMeans(int K);
        int get_nearest_cluster_id(Point point);
        void run(vector<Point>& all_points);
        int get_nearest_cluster(Point point);


};

#endif
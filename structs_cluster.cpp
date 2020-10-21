#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <stdlib.h>
#include <algorithm>
#include <random>
#include <limits>
#include "structs_cluster.h"
#include "structs.h"

random_device rd;
mt19937_64 eng(rd());

uniform_int_distribution<unsigned long long> distr;

using namespace std;

/* FUNCTIONS FOR CLUSTERING */

Point::Point(vector<unsigned char> image,int dimensions)
{
    this->image=image;
    this->dimensions=dimensions;
    this->clusterId = -1;
}

vector<unsigned char> Point::get_image()
{
    return image;
}

int Point::get_dimensions()
{
    return this->dimensions;
}

unsigned char Point::getPixel(int position)
{
    return this->image[position];
}

void Point::set_cluster(int id)
{
    this->clusterId=id;
}

int Point::get_cluster()
{
    return clusterId;
}

Cluster::Cluster(int clusterId,Point centroid)
{
    this->clusterId = clusterId;
    for(int i = 0; i < centroid.get_dimensions(); i++)
    {
        this->center.push_back(centroid.getPixel(i));
    }
    centroid.set_cluster(clusterId);
    this->images.push_back(centroid);
}

int Cluster::get_clusterId()
{
    return clusterId;
}

void Cluster::add_image(Point point)
{
    images.push_back(point);
}

vector<unsigned char> Cluster::get_center()
{
    return center;
}

void Cluster::calculate_center(int dimensions)
{
    vector <unsigned char> new_center;
    unsigned char pixel;
    unsigned long int t = 5;

    for(int i = 0; i < dimensions; i++)
    {
        int sum = 0;
        for(int j = 0; j < images.size(); j++)
        {
            //cout << (int)(images[j].getPixel(i));
            sum = sum + (int)(images[j].getPixel(i));
        }
        sum = sum/images.size();
        pixel = (unsigned char)(sum);
        new_center.push_back(pixel);
        //cout << (int)pixel << endl;
    }
    //cout << "new center size: " << new_center.size() << endl;
    this->center = new_center;
}

bool Cluster::remove(Point point)
{
    vector<unsigned char> image = point.get_image();
    int size = images.size();
    
    int image_number = get_image_pos(image);

    for(int i = 0; i < size; i++)
    {
        int temp_number = get_image_pos(images[i].get_image());
        if(temp_number == image_number)
        {
            images.erase(images.begin() + i);
            cout << "image " << image_number << " removed from cluster: " << this->clusterId << endl;
            return true;
        }
    }
    return false;
}

int Cluster::get_size()
{
    return images.size();
}

int Cluster::calculate_average_distance()
{
    int count = 0;
    int sum = 0;
    int dist;
    for(int i = 0; i < images.size(); i++)
    {
        for(int j = i+1; j < images.size(); j++)
        {
            dist = manhattan_dist(images[i].get_image(),images[j].get_image(),images[i].get_dimensions());
            sum = sum + dist;
            //cout << sum << endl;
            count++;
        }
    }
    return sum/count;
}



KMeans::KMeans(int k)
{
    this->K = k;
    cout<<"Kmeans class created" << endl;
}

int KMeans::get_nearest_cluster(Point point)
{
    int min_dist,nearest_cluster;
    
    min_dist = manhattan_dist(clusters[0].get_center(), point.get_image(),dimensions);
    nearest_cluster = clusters[0].get_clusterId();

    for(int i = 1; i < K; i++)
    {
        int dist = manhattan_dist(clusters[i].get_center(), point.get_image(),dimensions);

        if(dist < min_dist)
        {
            min_dist = dist;
            nearest_cluster = clusters[i].get_clusterId();
        }
    }

    return nearest_cluster;
}

bool KMeans::prob(int dist,unsigned long long int sum)
{


    unsigned long long int sum_pow = sum*sum;
    unsigned long long int x = distr(eng) % sum_pow;
    cout << "x: " << x << " sum: " << sum << " sum pow: " << sum_pow<< endl;
    if(x < (unsigned long long int)dist*(unsigned long long int)dist)
    {
        return true;
    }
    else
    {
        return false;
    }   
}

void KMeans::initialize(vector<Point>& all_points)
{
    number_of_points = all_points.size();
    dimensions = all_points[0].get_dimensions();
    vector<int> used;



    int count_centroids = 0;
    int index = rand() % number_of_points;
    cout << "index: " << index << endl;
    count_centroids++;
    used.push_back(index);
    all_points[index].set_cluster(0);

    Cluster cluster(0,all_points[index]);
    clusters.push_back(cluster);
    cout << " Initializing " << endl;
    // // Calculating the min sum;
    // for(int i = 0; i < all_points.size(); i++)
    // {
    //     int minDist;
    //     if(find(used.begin(),used.end(),get_image_pos(all_points[i].get_image())) == used.end())
    //     {
    //         minDist = manhattan_dist(all_points[i].get_image(),clusters[0].get_center(),dimensions);
    //         for(int j = 1; j < count_centroids; j++)
    //         {
    //             int newDist = manhattan_dist(all_points[i].get_image(),clusters[j].get_center(),dimensions);
    //             if(newDist < minDist)
    //             {
    //                 minDist = newDist;
    //             }
    //         }
    //         sum = sum + minDist;
    //     }
    //     cout << "image: " << i << " has minDist: " << minDist << endl;
    // }
    // cout << sum << endl;

    while(count_centroids != K)
    {
        unsigned long int sum = 0;
        // Calculating the min sum;
        for(int i = 0; i < all_points.size(); i++)
        {
            int minDist;
            //cout << "image numb: " << get_image_pos(all_points[i].get_image()) - 1 << endl;
            if(find(used.begin(),used.end(),get_image_pos(all_points[i].get_image()) - 1) == used.end())
            {
                minDist = manhattan_dist(all_points[i].get_image(),clusters[0].get_center(),dimensions);
                for(int j = 1; j < count_centroids; j++)
                {
                    int newDist = manhattan_dist(all_points[i].get_image(),clusters[j].get_center(),dimensions);
                    if(newDist < minDist)
                    {
                        minDist = newDist;
                    }
                }
                sum = sum + minDist;
                //cout << "image: " << i << " has minDist: " << minDist << endl;
            }
        }
        //cout << sum << endl;


        for(int i = 0; i < all_points.size(); i++)
        {
            cout << i << endl;
            int minDist,cluster_id;
            if(find(used.begin(),used.end(),get_image_pos(all_points[i].get_image()) - 1) == used.end())
            {
                minDist = manhattan_dist(all_points[i].get_image(),clusters[0].get_center(),dimensions);
                cluster_id = 0;
                for(int j = 1; j < count_centroids; j++)
                {
                    int newDist = manhattan_dist(all_points[i].get_image(),clusters[j].get_center(),dimensions);
                    if(newDist < minDist)
                    {
                        minDist = newDist;
                        cluster_id = j;
                    }
                }
                if(this->prob(minDist,sum) == true)
                {
                    used.push_back(i);
                    all_points[i].set_cluster(cluster_id);
                    Cluster cluster(cluster_id,all_points[i]);
                    clusters.push_back(cluster);
                    count_centroids++;
                    cout << "true " << endl;
                    break;
                }
            }
            else
            {
                cout << "This point already picked " << endl;
            }
            
        }
    }


}

void KMeans::run(vector<Point>& all_points)
{
    number_of_points = all_points.size();
    dimensions = all_points[0].get_dimensions();


    // Initializing Clusters

    // vector<int> used;
    // for(int i = 0; i < K; i++)
    // {
    //     while(true){
    //         int index = rand() % number_of_points; // getting a random point to be a centroid of a cluster
    //         if(find(used.begin(),used.end(),index) == used.end())
    //         {
    //             // index doesnt exist in the vector
    //             used.push_back(index);
    //             all_points[index].set_cluster(i);
    //             Cluster cluster(i,all_points[index]);
    //             clusters.push_back(cluster);
    //             cout << "Cluster: " << i << " picked for centroid image number: " << index << endl;
    //             break;
    //         }
    //     }
    // }

    // Add all points to a cluster
    int count_changes = 101;
    while(count_changes >10)
    {
        count_changes = 0;
        for(int i = 0; i < number_of_points; i++)
        {
            int current_cluster = all_points[i].get_cluster();
            int nearest_cluster = get_nearest_cluster(all_points[i]);

            if(current_cluster != nearest_cluster)
            {
                // se periptwsi poy einai hdh
                if(current_cluster != -1)
                {
                    for(int l = 0; l < K; l++)
                    {
                        if(clusters[l].get_clusterId() == current_cluster)
                        {
                            clusters[l].remove(all_points[i]);
                        }
                    }
                }

                for(int j = 0; j < K; j++)
                {
                    if(clusters[j].get_clusterId() == nearest_cluster)
                    {
                        clusters[j].add_image(all_points[i]);
                        cout << "image number: " << get_image_pos(all_points[i].get_image()) << " to cluster: " << nearest_cluster << endl;
                        count_changes++;
                        break;
                    }
                }
                all_points[i].set_cluster(nearest_cluster);
            }
        }
        //cout << "all images have been added to clusters " << endl;


        // Recalculating the centroid

        for(int i = 0; i < K; i++)
        {
            clusters[i].calculate_center(dimensions);
        }
    }

    for(int i = 0; i < K; i++)
    {
        cout << "CLUSTER-" << i << "{ size: " << clusters[i].get_size() << " }" << endl;
    }

}

void KMeans::lsh(vector<Point>& all_points,vector<Hash> hash_tables)
{
    number_of_points = all_points.size();
    dimensions = all_points[0].get_dimensions();
    // Initializing Clusters

    // cout << "lsh running" << endl;
    vector<int> used;
    for(int i = 0; i < K; i++)
    {
        while(true){
            int index = rand() % number_of_points; // getting a random point to be a centroid of a cluster
            if(find(used.begin(),used.end(),index) == used.end())
            {
                // index doesnt exist in the vector
                used.push_back(index);
                all_points[index].set_cluster(i);
                Cluster cluster(i,all_points[index]);
                clusters.push_back(cluster);
                cout << "Cluster: " << i << " picked for centroid image number: " << index << endl;
                break;
            }
        }
    }
    //int L = 10000;
    // Add similar points to the same cluster using range search
    for(int i = 0; i < K; i++)
    {
        vector<unsigned char> center = clusters[i].get_center();
        
        PQ pq_hash(center,2,hash_tables); // NA TO TSEKARW
        pq_hash.range_search(35000,hash_tables,center);
    }
    
}

void KMeans::silhouette()
{
    for(int i = 0; i < K; i++)
    {
        cout <<"Cluster-"<< i <<" average distance: "<< clusters[i].calculate_average_distance() << endl;;
    }
}




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

    center = centroid.get_image();

    //centroid.set_cluster(clusterId);
    //this->images.push_back(centroid);
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
    if(images.size() == 0)return;
    vector <unsigned char> new_center;
    new_center.clear();
    unsigned char pixel;
    unsigned long int t = 5;
    for(int i = 0; i < dimensions; i++)
    {
        vector <unsigned char> pixels;
        int sum = 0;
        for(int j = 0; j < images.size(); j++)
        {
            //cout << (int)(images[j].getPixel(i));
            //sum = sum + (int)(images[j].getPixel(i));
            pixels.push_back(images[j].getPixel(i));
        }
        sort(pixels.begin(),pixels.end());
        new_center.push_back(pixels[(images.size()/2)]); // SKAEI
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
            //cout << "image " << image_number << " removed from cluster: " << this->clusterId << endl;
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

void Cluster::display_images()
{
    for(int i = 0; i < images.size(); i++)
    {
        //cout << get_image_pos(images[i].get_image()) << endl;
        cout << images[i].get_cluster() << " position: " << get_image_pos(images[i].get_image()) - 1 << endl;
    }
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

    for(int i = 0; i < K; i++)
    {
        int dist = manhattan_dist(clusters[i].get_center(), point.get_image(),dimensions);
        //cout << "dist to cluster: "<< i << "-> " << dist << "min dist" << min_dist<<  endl;

        if(dist < min_dist)
        {
            min_dist = dist;
            nearest_cluster = i;
        }
    }

    return nearest_cluster;
}

int KMeans::prob(vector<unsigned long long int> dist)
{
//    cout<<"size is: "<<dist.size()<<endl;
    unsigned long long int x = distr(eng) % (dist.back());
    //cout<<x<<endl;
    int dist_size= dist.size(), i=0, mid;

    while(i != dist_size){
//        sleep(1);
        mid= i + ( (dist_size -i)/2 );
        if (dist[mid] < x){
            if(dist[mid+1] > x){
//                cout<<dist[mid]<<" between "<<dist[mid+1]<<"\t gonna return the right "<<mid+1<<endl;
                return mid+1;
            }
            i = mid + 1;

        }else if (dist[mid] > x){
            if(dist[mid-1] < x){
//                cout<<dist[mid]<<" between "<<dist[mid+1]<<"\t gonna return the right "<<mid+1<<endl;
                return mid+1;
            }
            dist_size = mid - 1;
        }else
            return mid;
    }
//    cout<<dist[i]<<" between "<<dist[i+1]<<"\t gonna return the right"<<i+1<<endl;
 
    return i+1;
}

void KMeans::initialize(vector<Point>& all_points)
{
    number_of_points = all_points.size();
    dimensions = all_points[0].get_dimensions();
    vector<int> used;



    int count_centroids = 0;
    int index = rand() % number_of_points;
    //cout << "index: " << index << endl;
    count_centroids++;
    used.push_back(index);
    //all_points[index].set_cluster(0);
    Cluster cluster(0,all_points[index]);
    clusters.push_back(cluster);
    cout << "Initializing " << endl;


    while(count_centroids != K)
    {
        unsigned long long int old;
        vector<unsigned long long int> dists;
        dists.push_back(0);
        for(int i = 0; i < all_points.size(); i++)
        {
            unsigned long long int minDist;
            int cluster_id;
            if(find(used.begin(),used.end(),get_image_pos(all_points[i].get_image()) - 1) == used.end())
            {
                minDist = manhattan_dist(all_points[i].get_image(),clusters[0].get_center(),dimensions);
                minDist = (unsigned long long int)minDist * (unsigned long long int)minDist;
                cluster_id = 0;
                for(int j = 1; j < count_centroids; j++)
                {
                    unsigned long long int newDist = manhattan_dist(all_points[i].get_image(),clusters[j].get_center(),dimensions);
                    newDist = (unsigned long long int)newDist*(unsigned long long int)newDist;
                    if(newDist < minDist)
                    {
                        minDist = (unsigned long long int)newDist;
                        cluster_id = j;
                    }
                }
                old=dists.back();
//                cout<<sqrt(minDist)<<endl;
                dists.push_back(minDist+old);
            }else{
                dists.push_back(1+old);
                //cout<<"Found something..."<<endl;
            }            
        }
        unsigned long long int xxx;
        //cout<<"PRINTING DI"<<endl;
         for(int e=1; e<dists.size() ; e++){
            xxx=dists[e] - dists[e-1];
            //cout<<sqrt(xxx)<<endl;
         }
        int pos = prob(dists);
        used.push_back(pos);
        Cluster cluster(count_centroids,all_points[pos]);
        clusters.push_back(cluster);
        count_centroids++;
    }
    int r = mean_centroid_distance();
}

void KMeans::run(vector<Point>& all_points, ofstream & out)
{
    number_of_points = all_points.size();
    dimensions = all_points[0].get_dimensions();

    cout << "Executing Lloyds " << endl;
    // Add all points to a cluster
    int count_changes = number_of_points;
    while(count_changes > number_of_points/30)
    {
        count_changes = 0;
        for(int i = 0; i < number_of_points; i++)
        {
            int current_cluster = all_points[i].get_cluster();
            int nearest_cluster = get_nearest_cluster(all_points[i]);
            //cout << "nearest cluster: " << nearest_cluster << endl;

            if(current_cluster != nearest_cluster)
            {
                // se periptwsi poy einai hdh
                if(current_cluster != -1)
                {
                    clusters[current_cluster].remove(all_points[i]);
                }
                clusters[nearest_cluster].add_image(all_points[i]);
                //cout << "image number: " << get_image_pos(all_points[i].get_image()) - 1 << " to cluster: " << nearest_cluster << endl;
                count_changes++;
            }
            all_points[i].set_cluster(nearest_cluster);
            //cout << "cluster: " << all_points[i].get_cluster() << endl;
            
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
        out << "CLUSTER-" << i+1 << "{ size: " << clusters[i].get_size() << " }" << endl;
        //clusters[i].display_images();

    }

}


// Lloyds for LSH
int KMeans::run_lsh(vector<Point>& all_points)
{
    number_of_points = all_points.size();
    dimensions = all_points[0].get_dimensions();
    int count_changes = 0;

    for(int i = 0; i < number_of_points; i++)
    {
        int current_cluster = all_points[i].get_cluster();
        int nearest_cluster = get_nearest_cluster(all_points[i]);
        //cout << "nearest cluster: " << nearest_cluster << endl;

        if(current_cluster != nearest_cluster)
        {
            // se periptwsi poy einai hdh
            if(current_cluster != -1)
            {
                clusters[current_cluster].remove(all_points[i]);
            }
            clusters[nearest_cluster].add_image(all_points[i]);
            count_changes++;
            //cout << "image number: " << get_image_pos(all_points[i].get_image()) - 1 << " to cluster: " << nearest_cluster << endl;
        }
        all_points[i].set_cluster(nearest_cluster);
        //cout << "cluster: " << all_points[i].get_cluster() << endl;
        
    }

    return count_changes;
}

int KMeans::mean_centroid_distance()
{
    int sum = 0;
    int second_cluster, dist;
    for(int i = 0; i < K; i++)
    {
        Point temp(clusters[i].get_center(),dimensions);
        second_cluster = get_second_nearest_cluster(temp,i);

        dist = manhattan_dist(clusters[i].get_center(),clusters[second_cluster].get_center(),dimensions);
        //cout << "distance: " << dist << endl;
        sum = sum + dist;
    }
    return sum/K;
}

void KMeans::lsh(vector<Point>& all_points,vector<Hash> hash_tables, ofstream & out)
{
    number_of_points = all_points.size();
    dimensions = all_points[0].get_dimensions();
    int count_changes = 1000;
    
    cout << "Reverse Assignment" << endl;
    int r = mean_centroid_distance()/2;
    while(r < 60000){
        //cout << "r: " << r << endl;
        count_changes = 0;
        // Add similar points to the same cluster using range search
        for(int i = 0; i < K; i++)
        {
            vector<unsigned char> center = clusters[i].get_center();
            
            PQ pq_hash(center,2, hash_tables); // NA TO TSEKARW
            //pq_hash.range_search(35000,hash_tables,center);
            vector<vector<unsigned char>> range_search = pq_hash.lsh_images_in_range(r,hash_tables,center);
            //cout << "range search size: " << range_search.size() << endl;

            for(int j = 0; j < range_search.size(); j++)
            {
                int pos = get_image_pos(range_search[j]) - 1;
                int current_cluster = all_points[pos].get_cluster();
                //cout << "current_cluster: " << current_cluster  << " Image:" << pos << endl;
                if(current_cluster != -1 && current_cluster != i) // This point is already in a cluster
                {
                    
                    //cout << all_points[pos].get_cluster() << endl;
                    int current_distance = manhattan_dist(all_points[pos].get_image(),clusters[current_cluster].get_center(),dimensions);
                    int new_distance = manhattan_dist(all_points[pos].get_image(),clusters[i].get_center(),dimensions);
                    if(new_distance < current_distance)
                    {
                        clusters[current_cluster].remove(all_points[pos]);
                        all_points[pos].set_cluster(i);
                        clusters[i].add_image(all_points[pos]);
                        //cout << "Image removed from: " << current_cluster << " and added to cluster: " << i << endl;
                        count_changes++;
                    }
                }
                else if(current_cluster == -1)
                {
                    all_points[pos].set_cluster(i);
                    clusters[i].add_image(all_points[pos]);
                    count_changes++;
                    // cout << "Image: "<< pos  << " added to: " << i << endl;
                    // cout << "New cluster: " << all_points[pos].get_cluster() << endl;
                }
                
            }
        }
        r = r*2;
    }

    cout << "Calling Lloyds" << endl;
    count_changes = count_changes + run_lsh(all_points);

    // for(int i = 0; i < K; i++)
    // {
    //     cout << "CLUSTER-" << i << "{ size: " << clusters[i].get_size() << " }" << endl;
    //     //clusters[i].display_images();
    // }
    

    cout << "Reverse Assignment" << endl;
    while(count_changes > number_of_points/80){
        r = mean_centroid_distance()/2;
        count_changes = 0;
        while(r < 60000){
            //cout << "r: " << r << endl;
            // Add similar points to the same cluster using range search
            for(int i = 0; i < K; i++)
            {
                vector<unsigned char> center = clusters[i].get_center();
                
                PQ pq_hash(center,2, hash_tables); // NA TO TSEKARW
                //pq_hash.range_search(35000,hash_tables,center);
                vector<vector<unsigned char>> range_search = pq_hash.lsh_images_in_range(r,hash_tables,center);
                //cout << "range search size: " << range_search.size() << endl;

                for(int j = 0; j < range_search.size(); j++)
                {
                    int pos = get_image_pos(range_search[j]) - 1;
                    int current_cluster = all_points[pos].get_cluster();
                    //cout << "current_cluster: " << current_cluster  << " Image:" << pos << endl;
                    if(current_cluster != -1 && current_cluster != i) // This point is already in a cluster
                    {
                        
                        //cout << all_points[pos].get_cluster() << endl;
                        int current_distance = manhattan_dist(all_points[pos].get_image(),clusters[current_cluster].get_center(),dimensions);
                        int new_distance = manhattan_dist(all_points[pos].get_image(),clusters[i].get_center(),dimensions);
                        if(new_distance < current_distance)
                        {
                            clusters[current_cluster].remove(all_points[pos]);
                            all_points[pos].set_cluster(i);
                            clusters[i].add_image(all_points[pos]);
                            //cout << "Image removed from: " << current_cluster << " and added to cluster: " << i << endl;
                            count_changes++;
                        }
                    }
                    else if(current_cluster == -1)
                    {
                        all_points[pos].set_cluster(i);
                        clusters[i].add_image(all_points[pos]);
                        count_changes++;
                        // cout << "Image: "<< pos  << " added to: " << i << endl;
                        // cout << "New cluster: " << all_points[pos].get_cluster() << endl;
                    }
                    
                }
            }
            r = r*2;
        }

        for(int i = 0; i < K; i++)
        {
            clusters[i].calculate_center(dimensions);
//            cout << "CLUSTER-" << i+1 << "{ size: " << clusters[i].get_size() << " }" << endl;
            //clusters[i].display_images();
        }
    }
    for(int i = 0; i < K; i++)
    {
        out << "CLUSTER-" << i+1 << "{ size: " << clusters[i].get_size() << " }" << endl;
        //clusters[i].display_images();
    }


}

void KMeans::hypercube(vector<Point>& all_points, Cube cube, int M, int probes, int k, ofstream & out){
    number_of_points = all_points.size();
    dimensions = all_points[0].get_dimensions();
    int count_changes = 1000;
    
    
    int r = mean_centroid_distance()/2;

    cout << "Reverse Assignment" << endl;
    while(r < 60000){
        //cout << "r: " << r << endl;
        count_changes = 0;
        // Add similar points to the same cluster using range search
        for(int i = 0; i < K; i++)
        {
            vector<unsigned char> center = clusters[i].get_center();
            
            PQ pq_cube(center,2,cube, M, probes, k); // NA TO TSEKARW
            //pq_hash.range_search(35000,hash_tables,center);
//            vector<vector<unsigned char>> range_search = pq_hash.lsh_images_in_range(r,hash_tables,center);

            vector<vector<unsigned char>> range_search = pq_cube.cube_images_in_range(r, cube, center, probes, k, M);
            //cout << "range search size: " << range_search.size() << endl;

            for(int j = 0; j < range_search.size(); j++)
            {
                int pos = get_image_pos(range_search[j]) - 1;
                int current_cluster = all_points[pos].get_cluster();
                //cout << "current_cluster: " << current_cluster  << " Image:" << pos << endl;
                if(current_cluster != -1 && current_cluster != i) // This point is already in a cluster
                {
                    
                    //cout << all_points[pos].get_cluster() << endl;
                    int current_distance = manhattan_dist(all_points[pos].get_image(),clusters[current_cluster].get_center(),dimensions);
                    int new_distance = manhattan_dist(all_points[pos].get_image(),clusters[i].get_center(),dimensions);
                    if(new_distance < current_distance)
                    {
                        clusters[current_cluster].remove(all_points[pos]);
                        all_points[pos].set_cluster(i);
                        clusters[i].add_image(all_points[pos]);
                        //cout << "Image removed from: " << current_cluster << " and added to cluster: " << i << endl;
                        count_changes++;
                    }
                }
                else if(current_cluster == -1)
                {
                    all_points[pos].set_cluster(i);
                    clusters[i].add_image(all_points[pos]);
                    count_changes++;
                    // cout << "Image: "<< pos  << " added to: " << i << endl;
                    // cout << "New cluster: " << all_points[pos].get_cluster() << endl;
                }
                
            }
        }
        r = r*2;
    }

    cout << "Calling Lloyds" << endl;
    count_changes = count_changes + run_lsh(all_points);

    // for(int i = 0; i < K; i++)
    // {
    //     clusters[i].calculate_center(dimensions);
    //     cout << "CLUSTER-" << i << "{ size: " << clusters[i].get_size() << " }" << endl;
    //     //clusters[i].display_images();
    // }



    cout << "Reverse Assignment" << endl;
    while(count_changes > number_of_points/80){
        r = mean_centroid_distance()/2;
        while(r < 60000){
            //cout << "r: " << r << endl;
            count_changes = 0;
            // Add similar points to the same cluster using range search
            for(int i = 0; i < K; i++)
            {
                vector<unsigned char> center = clusters[i].get_center();
                
                PQ pq_cube(center,2,cube, M, probes, k); // NA TO TSEKARW
                //pq_hash.range_search(35000,hash_tables,center);
    //            vector<vector<unsigned char>> range_search = pq_hash.lsh_images_in_range(r,hash_tables,center);

                vector<vector<unsigned char>> range_search = pq_cube.cube_images_in_range(r, cube, center, probes, k, M);
                //cout << "range search size: " << range_search.size() << endl;

                for(int j = 0; j < range_search.size(); j++)
                {
                    int pos = get_image_pos(range_search[j]) - 1;
                    int current_cluster = all_points[pos].get_cluster();
                    //cout << "current_cluster: " << current_cluster  << " Image:" << pos << endl;
                    if(current_cluster != -1 && current_cluster != i) // This point is already in a cluster
                    {
                        
                        //cout << all_points[pos].get_cluster() << endl;
                        int current_distance = manhattan_dist(all_points[pos].get_image(),clusters[current_cluster].get_center(),dimensions);
                        int new_distance = manhattan_dist(all_points[pos].get_image(),clusters[i].get_center(),dimensions);
                        if(new_distance < current_distance)
                        {
                            clusters[current_cluster].remove(all_points[pos]);
                            all_points[pos].set_cluster(i);
                            clusters[i].add_image(all_points[pos]);
                            //cout << "Image removed from: " << current_cluster << " and added to cluster: " << i << endl;
                            count_changes++;
                        }
                    }
                    else if(current_cluster == -1)
                    {
                        all_points[pos].set_cluster(i);
                        clusters[i].add_image(all_points[pos]);
                        count_changes++;
                        // cout << "Image: "<< pos  << " added to: " << i << endl;
                        // cout << "New cluster: " << all_points[pos].get_cluster() << endl;
                    }
                    
                }
            }
            r = r*2;
        }


        for(int i = 0; i < K; i++)
        {
            clusters[i].calculate_center(dimensions);
//            cout << "CLUSTER-" << i+1 << "{ size: " << clusters[i].get_size() << " }" << endl;
            //clusters[i].display_images();
        }
    }
    for(int i = 0; i < K; i++)
    {
//        clusters[i].calculate_center(dimensions);
        out << "CLUSTER-" << i+1 << "{ size: " << clusters[i].get_size() << " }" << endl;
        //clusters[i].display_images();
    }

    
}



int KMeans::get_second_nearest_cluster(Point point, int cluster_id)
{
    int min_dist,nearest_cluster;
    
    min_dist = 999999;

//    cout<<"My cluster is "<<cluster_id;

    for(int i = 0; i < K; i++)
    {   
        if (cluster_id != i ){
            int dist = manhattan_dist(clusters[i].get_center(), point.get_image(),dimensions);
//            cout << "dist to cluster: "<< i << "-> " << dist << "min dist" << min_dist<<  endl;

            if(dist < min_dist)
            {
                min_dist = dist;
                nearest_cluster = i;
            }
        }
    }

//    cout<<" and nearest cluster is "<<nearest_cluster<<endl;

    return nearest_cluster;
}

int Cluster::calculate_average_distance_silh(vector<unsigned char> image)
{
    int count = -1;
    int sum = 0;
    int dist;
    for(int i = 0; i < images.size(); i++)
    {
        dist = manhattan_dist(images[i].get_image(),image,images[i].get_dimensions());
        sum = sum + dist;
        //cout << sum << endl;
        count++;
    }
    return sum/count;
}

vector<Point> Cluster::get_images(){
    return images;
}

void KMeans::silhouette(ofstream & out)
{
    int number_of_images, meandist_nearest, meandist_second, second_cluster;
    int samples, j, count=0;
    vector<Point> images;
    vector<unsigned char> image;

    double s, sil=0, si;
    out<<"Silhouette: [";
    if (K<=1){
        out<<"0]"<<endl;
        return;
    }

    for(int i = 0; i < K; i++)
    {
        si=0;
        images=clusters[i].get_images();
        number_of_images=images.size();
        for (j=0; j<number_of_images ; j++){
            image=images[j].get_image();
            meandist_nearest=clusters[i].calculate_average_distance_silh(image);
            second_cluster=get_second_nearest_cluster(images[j], i);
            meandist_second=clusters[second_cluster].calculate_average_distance_silh(image);
            s=(double)meandist_second - (double)meandist_nearest;
            if (meandist_nearest>meandist_second)
                s = s / (double) meandist_nearest;
            else
                s = s / (double) meandist_second;
            
            sil+=s;
            si+=s;
            count++;
        }
        if(number_of_images != 0)
        {
            si = si / (double) number_of_images;
        }
        else
        {
            si = 0;
        }
        
        out<<si<<",";
    }
    sil=sil/(double) count;
    out<<sil<<"]"<<endl;
}

void KMeans::display(ofstream & out)
{
    for(int i = 0; i < K; i++)
    {
        out << "Cluster-" << i+1 << " {";
        vector<unsigned char> c = clusters[i].get_center();
        for(int j = 0; j < dimensions; j++)
        {
            out << int(c[j]);
        }
        cout << ", ";
        vector<Point> imgs = clusters[i].get_images();
        for(int k = 0; k < imgs.size(); k++)
        {
            out << "image_number_" << get_image_pos(imgs[k].get_image()) << ", ";
        }
        out << " }" << endl;
    }
}





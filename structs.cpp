
#include "structs.h"
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include "image.h"
#include "functions.h"

using namespace std;


// vector<unsigned char>* hash_list::get_image()
// {
    
// }

// image::image(vector<unsigned char> i)
// {
//     img = &i;
// }

void Hash_list::display_list()
{

    for (auto &l : list_of_images)
    {
        //cout << "list of images size: " << list_of_images.size();
        //cout << "->" <<  (unsigned int)l->at(0) << endl;

        cout << "-->" ;
        for(int i = 0; i < l->size()-3; i++)
        {
            cout << "-" << (unsigned int)l->at(i);
        }
    }
    cout << endl;
}

Hash_list::Hash_list()
{
    for (auto &l : list_of_images)
    {
        cout << l->empty();
    }
}

void Hash_list::add_image(vector<unsigned char> &i)
{
    //image im(i);
    list_of_images.push_back(&i);
}

void Hash_list::clear()
{
    for (auto &l : list_of_images)
    {
        l->clear();
        cout << "clear" << endl;
        cout << l->empty();
    }
}

void Hash_list::searchByKey()
{
    for (auto &l : list_of_images)
    {
        //cout << "list of images size: " << list_of_images.size();
        //cout << "->" <<  (unsigned int)l->at(0) << endl;

        cout << "-->" ;
        for(int i = 0; i < l->size()-3; i++)
        {
            cout << "-" << (unsigned int)l->at(i);
        }
    }
    cout << endl;
}

Hash::Hash(int b)
{
    this->bucket = b; //size of the hash table
    // table = new array<list< vector<unsigned char>>,bucket>;
    hash_table = new Hash_list[bucket];

    for(int i = 0; i < bucket; i++)
    {
        //cout << i << endl;;
        
        hash_table[i].clear();
    }
}

Hash::~Hash()
{
    delete[] hash_table;
}

void Hash::insertItem(vector<unsigned char> &image, int key)
{
    //int index = 2; // we call the hash function
    hash_table[key].add_image(image);
    //table[index].push_back(image);
}

void Hash::displayHash()
{
    list<vector<unsigned char>*>::iterator it;
    for(int i = 0; i < bucket; i++)
    {
        cout << i ;
        hash_table[i].display_list();
    }
}

void Hash::searchByKey(int index)
{
    // search by key
    hash_table[index].searchByKey();
}



/* FUNCTIONS FOR PQ */

image::image(int dist, vector<unsigned char> &i)
{
    distance = dist;
    img = &i;
}

int image::get_distance()
{
    return distance;
}


PQ::PQ(vector<vector<unsigned char>> imgs, vector<unsigned char> query, int N)
{
    //images = &imgs;
    maxDistance = 0;
    int dist, i, dimension, number_of_images;
    dimension=imgs[0].size()-3;
    number_of_images = imgs.size();

    for (i=0; i<N ; i++){
        dist= manhattan_dist(query, imgs[i], dimension);
        image temp(dist, imgs[i]);
        pq.push(temp);
    }
    image temp2=pq.top();            //we may need copy constructor
    maxDistance=temp2.get_distance();
    for(i=N; i<number_of_images ; i++){
        dist= manhattan_dist(query, imgs[i], dimension);
        image temp3 = pq.top();
        if (temp3.get_distance() > dist){
            pq.pop();
            pq.push(temp3);
            image temp2=pq.top();            //we may need copy constructor
            maxDistance=temp2.get_distance();
        }
    }


    // for (int i = 0; i < imgs.size(); i++)
    // {
    //     // calculate distance (query, image[i])
    //     // LEIPEI
    //     dist = 
        
    //     if(pq.size() < N)
    //     {
    //         image temp(dist, imgs[i]);
    //         pq.push(temp);
    //     }
    //     else
    //     {
    //         image temp2 = pq.top();
    //         if(temp2.get_distance() > dist)
    //         {
    //             pq.pop();
    //             pq.push(temp2);
    //         }
    //     }        
    // }
}


// For hash table
PQ::PQ(list<vector<unsigned char>*> b, vector<unsigned char> query, int N)
{
    maxDistance = 0;
    int dist, i, dimension, number_of_images;
    vector<unsigned char>* t = b.front();
    dimension = t->size()-3;
    number_of_images = b.size();

    // auto end = next(b.begin(), min(N, b.size()));
    // list<vector<unsigned char>*> first();

    int count = 0;
    for (auto &l : b)
    {
        dist= manhattan_dist(query, *l, dimension);
        if(pq.size() < N)
        {
            image temp(dist, *l);
            pq.push(temp);
        }
        else if(count == N)
        {
            image temp2=pq.top();            //we may need copy constructor
            maxDistance=temp2.get_distance();

            dist= manhattan_dist(query, *l, dimension);
            image temp3 = pq.top();
            if(temp3.get_distance() > dist)
            {
                pq.pop();
                pq.push(temp3);
                temp2=pq.top();
                maxDistance=temp2.get_distance();
            }
        }
        else
        {
            dist= manhattan_dist(query, *l, dimension);
            image temp3 = pq.top();
            if(temp3.get_distance() > dist)
            {
                pq.pop();
                pq.push(temp3);
                image temp2=pq.top();
                maxDistance=temp2.get_distance();
            }
        }
        count++;
    }
}

void PQ::displayN()
{
    while(!pq.empty())
    {
        image temp = pq.top();
        cout << "distance: " << temp.get_distance() << endl;
        pq.pop();
    }
}



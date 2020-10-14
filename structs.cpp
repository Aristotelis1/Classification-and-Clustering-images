
#include "structs.h"
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include "image.h"
#include "functions.h"
#include "hash_functions.h"

using namespace std;


// vector<unsigned char>* hash_list::get_image()
// {
    
// }

// image::image(vector<unsigned char> i)
// {
//     img = &i;
// }

list<vector<unsigned char>*> Hash_list::get_list_of_images()
{
    return list_of_images;
}

void Hash_list::display_list()
{

    for (auto const&l : list_of_images)
    {
        //cout << "list of images size: " << list_of_images.size();
        //cout << "->" <<  (unsigned int)l->at(0) << endl;

        cout << "-->" ;
        cout << l->at(0) << endl;
        // for(int i = 0; i < (l->size()-3); i++)
        // {
        //     cout << "-" << (unsigned int)l->at(i);
        // }
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

Hash::Hash(int number_of_images, vector<vector<unsigned char>> images, int dimension, int k, vector<double>s)
{
    this->bucket = number_of_images/8; //size of the hash table
    // table = new array<list< vector<unsigned char>>,bucket>;
    hash_table = new Hash_list[bucket];
    w=get_w(get_mean_range(number_of_images/30, images));
    for (int i=0; i<k; i++){
        Hash_Function *temp = new Hash_Function(dimension, s, k);
        hfunctions.push_back(*temp);
    }

    // calculate g


    // for(int i = 0; i < bucket; i++)
    // {
    //     //cout << i << endl;;
        
    //     hash_table[i].clear();
    // }

    cout <<"Hash table created " << endl;
}

Hash::~Hash()
{
    //delete[] hash_table;
}

int Hash::calculate_g(vector<unsigned char> img)
{
    testg=concatenate_h(hfunctions, img, w);
    int key = testg % (bucket);

    cout << "keyy: " << key << endl;

    return key;

}

list<vector<unsigned char>*> Hash::get_list_of_images()
{
    return hash_table->get_list_of_images();
}

void Hash::insertItem(vector<unsigned char> image)
{
    //int index = 2; // we call the hash function
    int key = calculate_g(image);
    //cout << "pixel 2: " << (int)image.at(2) << endl;
    //cout << "inserting item in: " << key << endl;
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
    cout << "searching key:" << index << endl;
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
    cout << temp2.get_distance() << "temp2" << endl;
    for(i=N; i<number_of_images ; i++){
        dist= manhattan_dist(query, imgs[i], dimension);
        image temp3(dist, imgs[i]);
        //cout << "temp3:" << temp3.get_distance() << endl;
        //cout << "dist: " << dist << endl;
        if (maxDistance > dist){
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
PQ::PQ(vector<unsigned char> query, int N, vector<Hash> hash_tables)
{
    maxDistance = 0;
    list<vector<unsigned char>*> list_of_images = hash_tables[0].get_list_of_images();


    // auto end = next(b.begin(), min(N, b.size()));
    // list<vector<unsigned char>*> first();

    int count = 0;
    
    // for (auto&l : list_of_images)
    // {
    //     cout << " lala " << endl;
    //     dist = manhattan_dist(query,*l, dimension);
    //     if(count < N)
    //     {
    //         image temp(dist, *l);
    //         pq.push(temp);
    //     }else if(count == N)
    //     {
    //         image temp2 = pq.top();
    //         maxDistance = temp2.get_distance();

    //         dist = manhattan_dist(query, *l, dimension);
    //         if(maxDistance > dist)
    //         {
    //             image temp3(dist,*l);
    //             pq.pop();
    //             pq.push(temp3);

    //             image temp4 = pq.top();
    //             maxDistance = temp4.get_distance();
    //         }
    //     }
    //     else
    //     {
    //         dist= manhattan_dist(query, *l, dimension);
    //         if(maxDistance > dist)
    //         {
    //             image temp3(dist,*l);
    //             pq.pop();
    //             pq.push(temp3);

    //             image temp4 = pq.top();
    //             maxDistance = temp4.get_distance();
    //         }
    //     }

    //     count++;  
    // }

    for(int i = 0; i < hash_tables.size(); i++)
    {
        list_of_images = hash_tables[i].get_list_of_images();
        int dist, dimension, number_of_images;
        vector<unsigned char>* t = list_of_images.front();
        dimension = t->size()-3;
        number_of_images = list_of_images.size();
        count = 0;

        for(auto &l : list_of_images)
        {
            cout << " checkkk :" << endl;
            cout << "check:" << (int)l->at(0) << endl;
            dist = manhattan_dist(query, *l, dimension);
            if(count < N)
            {
                image temp(dist,*l);
                pq.push(temp);
                image temp2 = pq.top();
                maxDistance = temp2.get_distance();
            }
            else{
                if(maxDistance > dist)
                {
                    image temp3(dist,*l);
                    pq.pop();
                    pq.push(temp3);

                    image temp4 = pq.top();
                    maxDistance = temp4.get_distance();
                }
            }
            count++;
        }

    }

    cout << "pq created " << endl;



    // for (auto &l : b)
    // {
    //     dist= manhattan_dist(query, *l, dimension);
    //     if(pq.size() < N)
    //     {
    //         image temp(dist, *l);
    //         pq.push(temp);
    //     }
    //     else if(count == N)
    //     {
    //         image temp2=pq.top();            //we may need copy constructor
    //         maxDistance=temp2.get_distance();

    //         dist= manhattan_dist(query, *l, dimension);
    //         image temp3 = pq.top();
    //         if(temp3.get_distance() > dist)
    //         {
    //             pq.pop();
    //             pq.push(temp3);
    //             temp2=pq.top();
    //             maxDistance=temp2.get_distance();
    //         }
    //     }
    //     else
    //     {
    //         dist= manhattan_dist(query, *l, dimension);
    //         image temp3 = pq.top();
    //         if(temp3.get_distance() > dist)
    //         {
    //             pq.pop();
    //             pq.push(temp3);
    //             image temp2=pq.top();
    //             maxDistance=temp2.get_distance();
    //         }
    //     }
    //     count++;
    // }
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

priority_queue<image> PQ::get_pq()
{
    return pq;
}



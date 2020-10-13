
#include "structs.h"
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include "image.h"

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
        for(int i = 0; i < l->size(); i++)
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
        for(int i = 0; i < l->size(); i++)
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

image::image(int dist, vector<unsigned char> i)
{
    distance = dist;
    img = &i;
}

int image::get_distance()
{
    return distance;
}




#include "structs.h"
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include "image.h"

using namespace std;


Hash::Hash(int b)
{
    this->bucket = b; //size of the hash table
    table = new list< vector<unsigned char> >[bucket];

}

Hash::~Hash()
{
    delete[] table;
}

void Hash::insertItem(vector<unsigned char> key)
{
    int index = 2; // we call the hash function
    table[index].push_back(key);
}

void Hash::displayHash()
{
    vector<unsigned char>::iterator vec_it;
    list< vector<unsigned char> >::iterator list_it;
    for(int i =0; i < bucket; i++)
    {
        cout << i;
        // for (vector<unsigned char> x : table[i])
        // {
        //     cout << " --> " << x.data();
        // }

        for(list_it = table[i].begin(); list_it != table[i].end(); ++ list_it)
        {
            //printf("lalaa\n");
            for(vec_it = list_it->begin(); vec_it != list_it->end(); ++vec_it)
            {
                //cout << vec_it;
                
                printf("------>%u", (unsigned)(*vec_it));
            }
        }
        cout << endl;

    }
}



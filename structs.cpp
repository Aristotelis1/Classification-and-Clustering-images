
#include "structs.h"
#include <string>
#include <iostream>
#include <list>
#include "image.h"

using namespace std;


Hash::Hash(int b)
{
    this->bucket = b; //size of the hash table
    table = new list<Image*>[bucket];

}

Hash::~Hash()
{
    delete[] table;
}

void Hash::insertItem(Image key)
{
    int index = 2; // we call the hash function
    table[index].push_back(&key);
}

void Hash::displayHash()
{
    for(int i =0; i < bucket; i++)
    {
        cout << i;
        for (auto x : table[i])
        {
            cout << " --> " << x->get_image();
        }
        cout << endl;

    }
}



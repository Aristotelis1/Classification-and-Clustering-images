
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

vector<vector<unsigned char>> Hash_list::get_list_of_images()
{
    return list_of_images;
}

void Hash_list::display_list()
{

    for (int i = 0; i < list_of_images.size(); i++)
    {
        //cout << "list of images size: " << list_of_images.size();
        //cout << "->" <<  (unsigned int)l->at(0) << endl;

        cout << "-->" ;
        //cout << l->at(0) << endl;
        // for(int i = 0; i < (l->size()-3); i++)
        // {
        //     cout << "-" << (unsigned int)l->at(i);
        // }
    }
    cout << endl;
}

Hash_list::Hash_list()
{
    list_of_images.clear();
}

void Hash_list::add_image(vector<unsigned char> &i)
{
    //image im(i);
    list_of_images.push_back(i);
}

void Hash_list::clear()
{
    // for (auto &l : list_of_images)
    // {
    //     l->clear();
    //     cout << "clear" << endl;
    //     cout << l->empty();
    // }
}

void Hash_list::searchByKey()
{
    // for (auto &l : list_of_images)
    // {
    //     //cout << "list of images size: " << list_of_images.size();
    //     //cout << "->" <<  (unsigned int)l->at(0) << endl;
    //     cout << "-->" ;
    //     for(int i = 0; i < l->size()-3; i++)
    //     {
    //         cout << "-" << (unsigned int)l->at(i);
    //     }
    // }
    // cout << endl;
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

//    cout << "keyy: " << key << endl;

    return key;

}

vector<vector<unsigned char>> Hash::get_list_of_images(int key)
{
    return hash_table[key].get_list_of_images();
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
    //list<vector<unsigned char>*>::iterator it;
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
    img = i;
}

image::image(const image &im2){
    distance = im2.distance;
    img = im2.img;

}

int image::get_distance()
{
    return distance;
}

vector<unsigned char> image::get_image(){
    return img;
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


}


// For hash table
PQ::PQ(vector<unsigned char> query, int N, vector<Hash> hash_tables,int r)
{
    int count = 0;
    int key;
    maxDistance = 0;
    vector<vector<unsigned char>> list_of_images;


    for(int i = 0; i < hash_tables.size(); i++)
    {
        key=hash_tables[i].calculate_g(query);
        list_of_images = hash_tables[i].get_list_of_images(key);
        int dist, dimension, number_of_images;
        number_of_images = list_of_images.size();

        for(int j = 0; j<number_of_images ; j++)
        {
//            cout << "check:" << (int)l->at(0) << endl;
            dist = manhattan_dist(query, list_of_images[j], list_of_images[j].size()-3);
            if(count < N)
            {
                image temp(dist,list_of_images[j]);
                pq.push(temp);
            }else if (count==N){
                image temp2 = pq.top();
                maxDistance = temp2.get_distance();
            }else{
                if(maxDistance > dist)
                {
                    image temp3(dist,list_of_images[j]);
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

    range_search(r,hash_tables,query);


}

void PQ::range_search(int r, vector<Hash> hash_tables, vector<unsigned char> query)
{
    int key;
    vector<vector<unsigned char>> list_of_images;
    int count = 0;
    for(int i = 0; i < hash_tables.size(); i++)
    {
        key=hash_tables[i].calculate_g(query);
        list_of_images = hash_tables[i].get_list_of_images(key);
        int dist, dimension, number_of_images;
        number_of_images = list_of_images.size();

        for(int j = 0; j<number_of_images ; j++)
        {
            dist = manhattan_dist(query, list_of_images[j], list_of_images[j].size()-3);
            if(dist < r)
            {
                image temp(dist,list_of_images[j]);
                count++;
                range.push(temp);
            }
            if(count > 50*hash_tables.size())
            {
                return;
            }
        }


    }
}

void PQ::displayN()
{
    while(!pq.empty())
    {
        image temp = pq.top();
        cout << "distance: " << temp.get_distance() << "\tin image position:"<< get_image_pos(temp.get_image()) << endl;
        pq.pop();
    }
    cout<<endl;
}

priority_queue<image> PQ::get_pq()
{
    return pq;
}

void display_prqueues(PQ pq_lsh, PQ pq_exhaust){
    int  i, lsh_size, n_size;
    priority_queue<image> lsh = pq_lsh.get_pq();
    priority_queue<image> exhaust = pq_exhaust.get_pq();
    
    // vector<image> vlsh(lsh.size());
    // vector<image> vexhaust(exhaust.size());

    vector<image> vlsh;
    vector<image> vexhaust;

    while(!lsh.empty()){
        image temp = (lsh.top());
        auto it = vlsh.insert(vlsh.begin(), temp);
        lsh.pop();
    }
    while (!exhaust.empty()){
        image temp = (exhaust.top());
        auto it = vexhaust.insert(vexhaust.begin(), temp);
        exhaust.pop();
    }
    n_size=vexhaust.size();
    lsh_size=vlsh.size();
    cout<<"n_size is: "<<n_size<<" and lsh_size is: "<<lsh_size<<endl;
    for (i=0; i<n_size ; i++){
        cout<<"Nearest neighbor-"<<i+1<<": ";
        cout<<get_image_pos(vexhaust[i].get_image())<<endl;

        cout<<"distanceLSH: ";
        //lsh distance
        if(i>=lsh_size)
            cout<<"neighboor not found"<<endl;
        else{
            cout<<vlsh[i].get_distance()<<endl;
        }

        cout<<"distanceTrue: ";
        cout<<vexhaust[i].get_distance()<<endl<<endl;
    }
}


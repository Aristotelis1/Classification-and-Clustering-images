
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

Hash::Hash(int number_of_images, vector<vector<unsigned char>> images, int dimension, int k, vector<double>s, int in_w)
{
    this->bucket = number_of_images/8; //size of the hash table
    // table = new array<list< vector<unsigned char>>,bucket>;
    hash_table = new Hash_list[bucket];
    w=in_w;
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

//    cout <<"Hash table created " << endl;

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
PQ::PQ(vector<unsigned char> query, int N, vector<Hash> hash_tables)
{
    int count = 0, z;
    int key;
    maxDistance = 0;
    vector<vector<unsigned char>> list_of_images;
    vector<int> impos;
    bool imexist;

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
                impos.push_back(get_image_pos(temp.get_image()));
            }else if (count==N){
                image temp2 = pq.top();
                maxDistance = temp2.get_distance();
            }else{
                if(maxDistance > dist)
                {
                    image temp3(dist,list_of_images[j]);
                    for (z=0, imexist=false ; z<impos.size() ; z++){
                        if (impos[z] == get_image_pos(temp3.get_image())){
                            imexist = true;
                            break;
                        }
                    }
                    if (!imexist){
                        pq.pop();
                        pq.push(temp3);
                        impos.push_back(get_image_pos(temp3.get_image()));
                        image temp4 = pq.top();
                        maxDistance = temp4.get_distance();
                    }
                }
            }
            count++;
        }

    }

    cout << "Priority queue searched:\t----"<<count<<" buckets----" << endl;

    // range_search(r,hash_tables,query);


}

void PQ::range_search(int r, vector<Hash> hash_tables, vector<unsigned char> query)
{
    int key, z;
    vector<vector<unsigned char>> list_of_images;
    vector<int>impos;
    bool imexist;
    int count = 0;
    cout << "r: " << r << endl;
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
                //image temp(dist,list_of_images[j]);
                int temp = get_image_pos(list_of_images[j]);
                for (z=0, imexist=false ; z < impos.size() ; z++){
                    if (impos[z] == temp){
                        imexist=true;
                        break;
                    }
                }
                if (!imexist){
                    count++;
                    range.push(temp);
                    impos.push_back(temp);
                }
            }
            if(count > 20*hash_tables.size())
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

void PQ::displayRange()
{
    //vector<int> imgs_pos;
    while(!range.empty())
    {
        int temp = range.top();
        cout << "image_number: "<< temp << endl;
        range.pop();
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
        if(i>=lsh_size)
        {
            cout<<"neighboor not found"<<endl;
        }else
        {
            cout<<get_image_pos(vlsh[i].get_image())<<endl;
        }

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

Cube::Cube (vector<vector<unsigned char>> images, int dimension, int k, vector<double>s, int in_w){
    vertices=pow(2,k);
    w=in_w;
    cube_vertex = new Hash_list[vertices];
    for (int i=0; i<k; i++){
        Hash_Function *temp = new Hash_Function(dimension, s, 4);
        hfunctions.push_back(*temp);
    }
    cout<<"Hypercube with "<<vertices<<" vertices created."<<endl;
}

int Cube::calculate_fh(int key){
    //TODO A GOOD F FOR SENDING HASH TO [0,1]
    return (key%2);

}



int Cube::calculate_vector_key(vector<unsigned char>image){
    int k, bit, fh=0, key;
    k=hfunctions.size();
    for (int i=0; i<k ; i++){
        key = hfunctions[i].get_hash_key(image, w);
        bit = calculate_fh(key);
        bit = bit << i;
        fh = fh | bit;
    }
    return fh;
}

void Cube::insertItem(vector<unsigned char> image)
{
    int key = calculate_vector_key(image);
    cube_vertex[key].add_image(image);
}


void Cube::displayCube()
{
    //list<vector<unsigned char>*>::iterator it;
    for(int i = 0; i < vertices; i++)
    {
        cout << i;
        cube_vertex[i].display_list();
    }
}

vector<vector<unsigned char>> Cube::get_list_of_images(int key)
{
    return cube_vertex[key].get_list_of_images();
}

PQ::PQ(vector<unsigned char> query, int N, Cube hypercube, int M, int probes, int k)
{
    int key, number_of_images, dist, i,j, z, count=0;
    bool fit = false, imexist;
    vector<vector<unsigned char>> list_of_images;
    vector<int> impos;
    vector<int> nb;
    
    key=hypercube.calculate_vector_key(query);
    list_of_images=hypercube.get_list_of_images(key);
    number_of_images = list_of_images.size();
    nb = get_route(k);
    if(probes>(pow(2,k)))   //dont have enough vertices to check
        probes=pow(2,k);    //changed probes to max vertices
    if (M < number_of_images)
        fit = true;
    if(fit == true){        //Search in only one vertex
        for (j=0 ; j<M ; j++){
            dist = manhattan_dist(query, list_of_images[j], list_of_images[j].size()-3);
            if (count < N){
                image temp(dist,list_of_images[j]);
                pq.push(temp);
                impos.push_back(get_image_pos(temp.get_image()));
            }else if (count==N){
                image temp2 = pq.top();
                maxDistance = temp2.get_distance();
            }else{
                if(maxDistance > dist)
                {
                    image temp3(dist,list_of_images[j]);
                    for (z=0, imexist=false ; z<impos.size() ; z++){
                        if (impos[z] == get_image_pos(temp3.get_image())){
                            imexist = true;
                            break;
                        }
                    }
                    if (!imexist){
                        pq.pop();
                        pq.push(temp3);
                        impos.push_back(get_image_pos(temp3.get_image()));
                        image temp4 = pq.top();
                        maxDistance = temp4.get_distance();
                    }
                }
            }
            count++;
        }
    }else{          //check other vertices
        int tempk=key;
        for(i=0 ; i<probes && count<M ; i++){
            list_of_images=hypercube.get_list_of_images(tempk);
            number_of_images = list_of_images.size();
            for (j=0 ; j<number_of_images && count<M ; j++){
                dist = manhattan_dist(query, list_of_images[j], list_of_images[j].size()-3);
                if (count < N){
                    image temp(dist,list_of_images[j]);
                    pq.push(temp);
                    impos.push_back(get_image_pos(temp.get_image()));
                }else if (count==N){
                    image temp2 = pq.top();
                    maxDistance = temp2.get_distance();
                }else{
                    if(maxDistance > dist)
                    {
                        image temp3(dist,list_of_images[j]);
                        for (z=0, imexist=false ; z<impos.size() ; z++){
                            if (impos[z] == get_image_pos(temp3.get_image())){
                                imexist = true;
                                break;
                            }
                        }
                        if (!imexist){
                            pq.pop();
                            pq.push(temp3);
                            impos.push_back(get_image_pos(temp3.get_image()));
                            image temp4 = pq.top();
                            maxDistance = temp4.get_distance();
                        }
                    }
                }
                count++;
            }
            // cout<<"OLD KEY: "<<key;
            tempk=change_neighbor(key, i+1, k, nb);
            // cout<<" CHANGED TO:"<<tempk<<endl;
        }

    }
//    cout<<"---------Totally checked "<<count<<"---------possible neighboors"<<endl;
 
}
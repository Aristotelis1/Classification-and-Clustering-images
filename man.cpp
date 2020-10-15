#include <iostream> 
#include <string.h>
#include <fstream>
#include <vector>

#include "hash_functions.h"
#include "structs.h"

using namespace std;


int main(int argc, char* argv[]) 
{ 
    srand(time(NULL));
    int i,y,z, k=-1, L=-1, N=-1;
    float R=-1.0;
    char input_file[128], query_file[128], output_file[128];

    for (i=1; i<argc ; i+=2)
    {

        if (strcmp(argv[i],"-d")==0){
            strcpy(input_file,argv[i+1]);
        }else if (strcmp(argv[i],"-q")==0){
            strcpy(query_file,argv[i+1]);
        }else if (strcmp(argv[i],"-k")==0){
            k=atoi(argv[i+1]);
        }else if (strcmp(argv[i],"-L")==0){
            L=atoi(argv[i+1]);
        }else if (strcmp(argv[i],"-o")==0){
            strcpy(output_file,argv[i+1]);
        }else if (strcmp(argv[i],"-N")==0){
            N=atoi(argv[i+1]);
        }else if (strcmp(argv[i],"-R")==0){
            R=atoi(argv[i+1]);
        // }else if (strcmp(argv[i],"-t")==0){       //lsh or cube
        //     cout << "Working on lsh..." << endl;        //TO BE DONE SOMETHING
        // }else if (strcmp(argv[i],"-M")==0){
        //     M=atoi(argv[i+1]);
        }else {
            cout << "Wrong arguments, please try again..." <<endl;
            return 0;
        }

    }

    while (strcmp(input_file, "")==0){
        cout <<"Need input file: ";
        cin >> input_file;
        cout <<endl;
    }while (strcmp(query_file, "")==0){
        cout <<"Need query file: ";
        cin >> query_file;
        cout <<endl;
    }while (strcmp(output_file, "")==0){
        cout <<"Need output file: ";
        cin >> output_file;
        cout <<endl;
    }
    while (k<2 || k>8){
        cout <<"Give me k in range 2-8 (if you want it on default type '0') : ";
        cin >> k;
        if (k==0){
            k=4;
            break;
        }
    }
    while (L<1 || L>20){
        cout <<"Give me L in range 1-20 (if you want it on default type '0') : ";
        cin >> L;
        if (L==0){
            L=5;
            break;
        }
    }
    while (N<1 || N>1000){
        cout <<"Give me N in range 1-1000 (if you want it on default type '0') : ";
        cin >> N;
        if (N==0){
            N=1;
            break;
        }
    }
    while (R<0.1 || R>1000){
        cout <<"Give me R (double) in range 0.1-1000 (if you want it on default type '0') : ";
        cin >> R;
        if (R==0){
            R=1.0;
            break;
        }
    }
    //Reading data from binary 


    cout << input_file << endl;

    std::ifstream file (input_file);

    int magic_number, number_of_images, rows, columns, dimension;
    unsigned char temp = -1;

    if (file.is_open()){
        file.read((char*)&magic_number,sizeof(magic_number)); 
        magic_number= change_endianess(magic_number);
        file.read((char*)&number_of_images,sizeof(number_of_images));
        number_of_images= change_endianess(number_of_images);
        file.read((char*)&rows,sizeof(rows));
        rows= change_endianess(rows);
        file.read((char*)&columns,sizeof(columns));
        columns= change_endianess(columns);
        dimension=rows*columns;

        //declare vector of images
        vector<vector<unsigned char>> images(number_of_images);
        int sum=0;
        unsigned char byte1, byte2, byte3;
        for(i = 0; i < number_of_images; ++i){
            sum=0;
            images[i].resize(dimension);
            for(y=0; y<dimension; ++y){
                file.read((char*)&temp,sizeof(temp));
                images[i][y]=temp;

            }
            get_bytes_from_int (i, byte1, byte2, byte3);        //split int (pos) to 3 bytes and push them at the end of the image vertex
            images[i].push_back(byte1);
            images[i].push_back(byte2);
            images[i].push_back(byte3);

        }
        cout << "Read binary file, with number_of_images = " << number_of_images << " and dimension = " << dimension << endl;
        cout << "Mean distance (r) is: " << get_mean_range(50, images) << endl;

        //create a vector for s (normally distributed L*k*d doubles in range [0,w])
        vector<double>s(L*k*dimension);
        double s_range = (double) get_w(get_mean_range(number_of_images/30, images)) / (double) (L * k * dimension);
        for (int i=0; i<L*k*dimension ; i++){
            s[i]= i*s_range;
        }

        // vector<Hash_Function>hfunctions;
        // // hfunctions.resize(k);
        // int w=get_w(get_mean_range(number_of_images/30, images));
        // for (i=0; i<k; i++){
        //     Hash_Function *temp = new Hash_Function(dimension, s, k);
        //     hfunctions.push_back(*temp);
        // }

        // Hash_Function hf(dimension, s, 2);
        // int w=get_w(get_mean_range(number_of_images/30, images));
//        cout<<"Hash key is: "<< hf.get_hash_key(images[100], w)<<endl;;

        //Hash h(number_of_images);

        //vector<Hash> 
        // unsigned long int testg;
        // for (i=1; i<100; i++){
        //     testg=concatenate_h(hfunctions, images[i], w);
        //     int key = testg % (number_of_images/8);
        //     h.insertItem(images[i], key);
        //     cout<< testg<<endl<<endl;
        // }
        // h.displayHash();



        
        // cout << "search by key 7487" << endl;
        // h.searchByKey(7487);


        // CREATING HASH TABLES

        vector<Hash> hash_tables;
        for(int j = 0; j < L; j++)
        {
            Hash h(number_of_images,images,dimension,k,s);
            hash_tables.push_back(h);
        }
        for(int j = 0; j < number_of_images; j++)
        {
            for(int i=0;i<L;i++)
            {
                hash_tables[i].insertItem(images[j]);
            }
            cout<<j<<endl;
        }




        //start reading the query file
        std::ifstream q_file (query_file);
        temp = -1;
        vector<unsigned char> query(dimension);
        if (q_file.is_open()){
            for (i=1; i<3 ; i++){          //read 10 queries now -> TODO until EOF
                // cout<<endl;
                for(y=0; y<dimension; ++y){         //read "query-image" on query (vector)
                    q_file.read((char*)&temp,sizeof(temp));
                    query[y]=temp;
                    // cout<<(int)query[y]<<"-";
                }
                // cout<<"Going to display exhaust..."<<endl;
                 PQ pr(images,query,N);
                // pr.displayN();
                // cout<<"Going to display lsh..."<<endl;
                 PQ pq_hash(query,N,hash_tables,45000);          
                // pq_hash.displayN();


                display_prqueues(pq_hash, pr);
                pq_hash.displayRange();

                //ASTO ETSI EINAI OI EKTYPWSEIS GIA META
                

                // cout<<"Query: "<<i+1<<endl;
                // for (i=1; i<=N ; i++){
                //     cout<<"Nearest neighbor-"<<i<<": ";
                //     //image number in dataset

                //     cout<<"distanceLSH: ";
                //     //lsh distance

                //     cout<<"distanceTrue"
                // }
                // cout<<"tLSH: ";
                // //lsh timer

                // cout<<"tTrue: ";
                // //true timer

                // cout<<R<<"-near neighbors:"
                // //cout all neighbors in range R
            }

        }else{
            cout<<"Cannot open query file: "<<query_file<<endl;
        }



    }else{
        cout<<"Cannot open input file: "<<input_file<<endl;
    }
    // unsigned char b1, b2, b3;
    // get_bytes_from_int(125555, b1, b2, b3);
    // cout<<(int)b1<<"-"<<(int)b2<<"-"<<(int)b3<<endl;

    // int target;
    // target= set_int_from_bytes(1 , 234, 115);
    // cout<<target<<endl;

    return 0; 



} 
#include <iostream> 
#include <string.h>
#include <fstream>
#include <vector>
#include <chrono>

#include "hash_functions.h"
#include "structs.h"

using namespace std;
using namespace std::chrono;


int main(int argc, char* argv[]) 
{ 
    srand(time(NULL));
    int i,y,z, k=-1, L=-1, N=-1, samples=100;
    float R=-1.0;
    char input_file[128], query_file[128], output_file[128];
    bool finished=false;

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
        }else if (strcmp(argv[i],"-t")==0){       // optional parameter for samples to get mean nearest-neighboor range
            samples=atoi(argv[i+1]);
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
    while (R<0.1 || R>70000){
        cout <<"Give me R (double) in range 0.1-70000 (if you want it on default type '0') : ";
        cin >> R;
        if (R==0){
            R=1.0;
            break;
        }
    }
    //Reading data from binary 


    std::ifstream file (input_file);
    std::ofstream out (output_file);

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

//      number_of_images=10000;       //uncomment this to read less images


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
        int w;
        w=get_w(get_mean_range(samples, images));
        //create a vector for s (normally distributed L*k*d doubles in range [0,w])
        vector<double>s(L*k*dimension);
        double s_range = (double) w / (double) (L * k * dimension);
        for (int i=0; i<L*k*dimension ; i++){
            s[i]= i*s_range;
        }

        // CREATING HASH TABLES

        vector<Hash> hash_tables;
        for(int j = 0; j < L; j++)
        {
            Hash h(number_of_images,images,dimension,k,s, w);
            hash_tables.push_back(h);
        }
        for(int j = 0; j < number_of_images; j++)
        {
            for(int i=0;i<L;i++)
            {
                hash_tables[i].insertItem(images[j]);
            }
//            cout<<j<<endl;
        }




        //start reading the query file
        while(finished==false){
            std::ifstream q_file (query_file);
            temp = -1;
            vector<unsigned char> query(dimension);

            // Measure execution time
            //auto start = high_resolution_clock::now();
            double lsh_duration = 0.0;
            double exhaust_duration = 0.0;
            if (q_file.is_open()){
                if(out.is_open()){
                    q_file.read((char*)&temp,sizeof(temp));
                    query[0]=temp;
                    for (i=1; i<10 ; i++){          //change "q_file" --> "i<10" to read less queries
                        // cout<<endl;
                        for(y=1; y<dimension; ++y){         //read "query-image" on query (vector)
                            q_file.read((char*)&temp,sizeof(temp));
                            query[y]=temp;
                            // cout<<(int)query[y]<<"-";
                        }
                        // cout<<"Going to display exhaust..."<<endl;

                        out<<"Query: "<<i<<endl;

                        auto start = high_resolution_clock::now();
                        PQ pr(images,query,N);
                        auto end = high_resolution_clock::now();
                        duration<double> elapsed_seconds = (end-start);
                        
                        exhaust_duration = exhaust_duration + elapsed_seconds.count();

                        // pr.displayN();
                        // cout<<"Going to display lsh..."<<endl;

                        auto start1 = high_resolution_clock::now();
                        PQ pq_hash(query,N,hash_tables); 
                        auto end1 = high_resolution_clock::now();  
                        duration<double> elapsed_seconds1 = (end1-start1);
                        
                        lsh_duration = lsh_duration + elapsed_seconds1.count();       
                        // pq_hash.displayN();
                        display_prqueues(pq_hash, pr, "LSH", out);
                        out << "tLSH: " << elapsed_seconds.count() << endl;
                        out << "tTrue: " << elapsed_seconds1.count() << endl;



                        pq_hash.range_search(R,hash_tables,query);
                        out << "R-near neighbors: " << endl;
                        pq_hash.displayRange(out);
                    }
                }else{
                    cout<<"Cannot open output_file: "<<output_file<<endl<<"Try again please: ";
                    cin>>output_file;
                }
            }else{
                cout<<"Cannot open query file: "<< query_file<<endl;
            }
            cout<<"If you want to finish press 0, else give me the file: ";
            cin>>query_file;
            if(strcmp("0", query_file)==0){
                q_file.close();
                finished=true; 
            }    
        }
    }else{
        cout<<"Cannot open input file: "<<input_file<<endl;
    }
    file.close();
    out.close();

    return 0; 



} 
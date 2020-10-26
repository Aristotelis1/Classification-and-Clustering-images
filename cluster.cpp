#include <iostream> 
#include <string.h>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <limits>


#include "functions.h"
#include "structs.h"
#include "structs_cluster.h"


using namespace std;
using namespace std::chrono;


int main(int argc, char* argv[])
{
    //srand(time(NULL));

    int k=-1;
    char input_file[128], config_file[128], output_file[128], method[15], complete[10];
    for (int i=1; i<argc ; i+=2)
    {
        if (strcmp(argv[i],"-i")==0){
            strcpy(input_file,argv[i+1]);
        }else if (strcmp(argv[i],"-c")==0){
            strcpy(config_file,argv[i+1]);
        }else if (strcmp(argv[i],"-method")==0){
            strcpy(method,argv[i+1]);
        }else if (strcmp(argv[i],"-o")==0){
            strcpy(output_file,argv[i+1]);
        }else if(strcmp(argv[i],"-complete")==0){
            strcpy(complete,argv[i+1]);
        }else {
            cout << "Wrong arguments, please try again..." <<endl;
            return 0;
        }
    }

    while (strcmp(input_file, "")==0){
        cout <<"Need input file: ";
        cin >> input_file;
        cout <<endl;
    }
    while (strcmp(config_file, "")==0){
        cout <<"Need config file: ";
        cin >> config_file;
        cout <<endl;
    }
    while (strcmp(output_file, "")==0){
        cout <<"Need output file: ";
        cin >> output_file;
        cout <<endl;
    }
    while (strcmp(method, "classic")!=0 && strcmp(method, "lsh")!=0 && strcmp(method, "hypercube")!=0){
        cout <<"Give me valid method (classic, lsh, hypercube): ";
        cin >> method;
        cout <<endl;
    }

    //READING CONFIG FILE

    std::ifstream config (config_file);
    char str[128];
    int L=3, k_lsh=4, M_hypercube=10, k_hypercube=3, probes=2;
    if (config.is_open()){
        while (config >> str){
            if (strcmp(str, "number_of_clusters:")==0){
                config >> str;
                k=atoi(str);
                
            }else if (strcmp(str, "number_of_vector_hash_tables:")==0){
                config >> str;
                L=atoi(str);

            }else if (strcmp(str, "number_of_vector_hash_functions:")==0){
                config >> str;
                k_lsh=atoi(str);

            }else if (strcmp(str, "max_number_M_hypercube:")==0){
                config >> str;
                M_hypercube=atoi(str);

            }else if (strcmp(str, "number_of_hypercube_dimensions:")==0){
                config >> str;
                k_hypercube=atoi(str);

            }else if (strcmp(str, "number_of_probes:")==0){
                config >> str;
                probes=atoi(str);

            }else{
                cout<<"Bad config texting. Please try again."<<endl;
                break;
            } 
        }

    }else{
        cout<<"Cannot open config file. Start over... Bye bye"<<endl;
        return 1;
    }

//    cout<< probes << k_hypercube << k_lsh << M_hypercube << k << L <<endl;
//    sleep(10);
    while (k<1){
        cout <<"Give me k > 1 : ";
        cin >> k;
    }

    config.close();

    //STARTING READING INPUT FILE

    std::ifstream file (input_file);
    std::ofstream out (output_file);

    int magic_number, number_of_images, rows, columns, dimension;
    unsigned char temp = -1;

    if(out.is_open()){
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

            number_of_images = 1000;

            //declare vector of images
            vector<Point> all_points;
            vector<vector<unsigned char> > images(number_of_images);
            int sum=0;
            unsigned char byte1, byte2, byte3;
            for(int i = 0; i < number_of_images; ++i){
                sum=0;
                images[i].resize(dimension);
                for(int y=0; y<dimension; ++y){
                    file.read((char*)&temp,sizeof(temp));
                    images[i][y]=temp;

                }
                get_bytes_from_int (i, byte1, byte2, byte3);        //split int (pos) to 3 bytes and push them at the end of the image vertex
                images[i].push_back(byte1);
                images[i].push_back(byte2);
                images[i].push_back(byte3);
                Point temp(images[i],images[i].size()-3);
                all_points.push_back(temp);

            }
            cout << "Read binary file, with number_of_images = " << number_of_images << " and dimension = " << dimension << endl;

            if(strcmp(method,"classic") == 0)
            {
                cout << "We gonna do classic clustering" << endl;
                out<< "Algorithm: Lloyds" << endl;
                auto start = high_resolution_clock::now();
                KMeans kmeans(k);
                kmeans.initialize(all_points);
                kmeans.run(all_points, out);
                auto end = high_resolution_clock::now();
                duration<double> elapsed_seconds = (end-start);
                out << "Clustering time: " << elapsed_seconds.count() << " seconds" << endl;
                kmeans.silhouette(out);

                if(strcmp(complete,"yes") == 0)
                {
                    kmeans.display(out);
                }
            }
            else if(strcmp(method,"lsh") == 0)
            {
                out<< "Algorithm: Range Search LSH" << endl;
                int samples = 100;

                // cout << "Give the number(L) of Hash Tables:" << endl;
                // cin >> L;

                int w=get_w(get_mean_range(samples, images));

                vector<double>s(L*k_lsh*dimension);
                double s_range = (double) w / (double) (L * k_lsh * dimension);
                for (int i=0; i<L*k_lsh*dimension ; i++){
                    s[i]= i*s_range;
                }

                vector<Hash> hash_tables;
                for(int j = 0; j < L; j++)
                {
                    Hash h(number_of_images,images,dimension,k_lsh,s, w);
                    hash_tables.push_back(h);
                }
                for(int j = 0; j < number_of_images; j++)
                {
                    for(int i=0;i<L;i++)
                    {
                        hash_tables[i].insertItem(images[j]);
                    }
                }

                auto start = high_resolution_clock::now();
                KMeans kmeans(k);
                kmeans.initialize(all_points);
                kmeans.lsh(all_points,hash_tables, out);
                //kmeans.run_lsh(all_points);

                auto end = high_resolution_clock::now();
                duration<double> elapsed_seconds = (end-start);
                out << "Clustering time: " << elapsed_seconds.count() << " seconds" << endl;
                kmeans.silhouette(out);


                if(strcmp(complete,"yes") == 0)
                {
                    kmeans.display(out);
                }
            }        
            else if(strcmp(method,"hypercube") == 0)
            {
                out<< "Algorithm: Range Search Hypercube" << endl;
                int samples=100;
                int w=get_mean_range(samples, images);

                //create a vector for s (normally distributed L*k*d doubles in range [0,w])
                vector<double>s(k_hypercube*dimension);
                double s_range = (double) w / (double) (k_hypercube * dimension);
                for (int i=0; i<(k_hypercube*dimension) ; i++){
                    s[i]= i*s_range;
                }
                Cube hypercube(images, dimension, k_hypercube, s, w);
                for(int j = 0; j < number_of_images; j++)
                {
                    hypercube.insertItem(images[j]);
        //            cout<<j<<endl;
                }
        //        hypercube.displayCube();

                auto start = high_resolution_clock::now();
                KMeans kmeans(k);
                kmeans.initialize(all_points);


                //TODO HYPERCUBE RUN
                kmeans.hypercube(all_points, hypercube, M_hypercube, probes, k_hypercube, out);
                //kmeans.run_lsh(all_points);

                //MAYBE ABOVE IMPLEMENTATION IS INCORRECT

                auto end = high_resolution_clock::now();
                duration<double> elapsed_seconds = (end-start);
                out << "Clustering time: " << elapsed_seconds.count() << " seconds" << endl;
                kmeans.silhouette(out);

                if(strcmp(complete,"yes") == 0)
                {
                    kmeans.display(out);
                }
            }

        }else{
            cout<<"Cannot open input file: "<<input_file<<endl;
        }
    }else{
        cout<<"Cannot open output file: "<<output_file<<endl;
    }
    file.close();
    out.close();

    return 0;
}
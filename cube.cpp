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
    int i,y,z, k=-1, M=-1, N=-1, probes;
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
        }else if (strcmp(argv[i],"-M")==0){
            M=atoi(argv[i+1]);
        }else if (strcmp(argv[i],"-o")==0){
            strcpy(output_file,argv[i+1]);
        }else if (strcmp(argv[i],"-N")==0){
            N=atoi(argv[i+1]);
        }else if (strcmp(argv[i],"-R")==0){
            R=atoi(argv[i+1]);
        }else if (strcmp(argv[i],"-probes")==0){
            probes=atoi(argv[i+1]);
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
            k=3;
            break;
        }
    }
    while (M<1 || M>1000){
        cout <<"Give me M in range 1-1000 (if you want it on default type '0') : ";
        cin >> M;
        if (M==0){
            M=10;
            break;
        }
    }while (probes<1 || probes>100){
        cout <<"Give me probes in range 1-100 (if you want it on default type '0') : ";
        cin >> probes;
        if (probes==0){
            probes=2;
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
    while (R<0.1 || R>50000){
        cout <<"Give me R (double) in range 0.1-50000 (if you want it on default type '0') : ";
        cin >> R;
        if (R==0){
            R=1.0;
            break;
        }
    }
    //Reading data from binary 
    if (probes>(pow(2, k))){
        probes=(pow(2, k));
        cout<<"Probes set to "<<probes<<", because of lack of vertices"<<endl;
    }


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
        vector<double>s(k*dimension);
        double s_range = (double) get_w(get_mean_range(number_of_images/30, images)) / (double) (k * dimension);
        for (int i=0; i<k*dimension ; i++){
            s[i]= i*s_range;
        }



    }else{
        cout<<"Cannot open input file: "<<input_file<<endl;
    }

    return 0; 



} 
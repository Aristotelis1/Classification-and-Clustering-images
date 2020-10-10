#include <iostream> 
#include <string.h>


using namespace std;
  
int main(int argc, char* argv[]) 
{ 
    int i, k=3, L=5, N=1, M=10;
    float R=1.0;
    string input_file, query_file, output_file;

    cout << "You have entered " << argc 
         << " arguments:" << "\n"; 
  
    for (int i = 0; i < argc; ++i) 
        cout << argv[i] << "\n"; 



    // for (i=1; i<argc ; i+=2)
    // {

    //     if (argv[i].compare("-d")==0){
    //         input_file=argv[i+1];
    //     }else if (argv[i].compare("-q")==0){
    //         query_file=argv[i+1];
    //     }else if (argv[i].compare("-k")==0){
    //         k=atoi(argv[i+1]);
    //     }else if (argv[i].compare("-L")==0){
    //         L=k=atoi(argv[i+1]);
    //     }else if (argv[i].compare("-o")==0){
    //         output_file=argv[i+1];
    //     }else if (argv[i].compare("-N")==0){
    //         N=atoi(argv[i+1]);
    //     }else if (argv[i].compare("-R")==0){
    //         R=atoi(argv[i+1]);
    //     }else if (argv[i].compare("-t")==0){       //lsh or cube
    //         input_file=argv[i+1];
    //     }else if (argv[i].compare("-M")==0){
    //         M=atoi(argv[i+1]);
    //     }else {
    //         cout << "Lathos orismata" << endl;
    //     }

    //}

    return 0; 
} 
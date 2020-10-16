# project1

g++ -o main man.cpp structs.cpp functions.cpp image.cpp hash_functions.cpp --std=c++11

./main -d train-images-idx3-ubyte -q t10k-images-idx3-ubyte -k 4 -L 4 -o output.txt


int get_image_pos (vector<unsigned char>image){
    int size=image.size();
    unsigned char byte3=image[size];
    unsigned char byte2=image[size-1];
    unsigned char byte1=image[size-2];
    return set_int_from_bytes(byte1, byte2, byte3);
}
    

./CLUSTER

g++ -o cluster cluster.cpp functions.cpp structs.cpp hash_functions.cpp --std=c++11

./cluster -i train-images-idx3-ubyte -c cluster.config -o output.txt -complete yes -method classic

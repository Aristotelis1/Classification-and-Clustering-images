# project1

g++ -o main man.cpp structs.cpp functions.cpp image.cpp hash_functions.cpp --std=c++11

./main -d train-images-idx3-ubyte -q t10k-images-idx3-ubyte -k 4 -L 4 -o output.txt

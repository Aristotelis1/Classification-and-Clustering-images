OBJS	= functions.o image.o man.o structs.o hash_functions.o
SOURCE	= functions.cpp image.cpp man.cpp structs.cpp hash_functions.cpp
HEADER	= functions.h image.h structs.h hash_functions.h
OUT	= test
CC	 = g++ --std=c++11
FLAGS	 = -g -c -Wall 
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

image.o: image.cpp image.h
	$(CC) $(FLAGS) image.cpp 

structs.o: structs.cpp structs.h
	$(CC) $(FLAGS) structs.cpp 

hash_functions.o: hash_functions.cpp hash_functions.h
	$(CC) $(FLAGS) hash_functions.cpp 

functions.o: functions.cpp functions.h
	$(CC) $(FLAGS) functions.cpp 

man.o: man.cpp
	$(CC) $(FLAGS) man.cpp 


clean:
	rm -f $(OBJS) $(OUT)
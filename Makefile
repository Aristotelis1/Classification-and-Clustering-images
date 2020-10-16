OBJS1	= functions.o image.o man.o structs.o hash_functions.o
SOURCE1	= functions.cpp image.cpp man.cpp structs.cpp hash_functions.cpp
HEADER1	= functions.h image.h structs.h hash_functions.h
OUT1	= lsh

OBJS2	= functions.o image.o structs.o hash_functions.o cube.o
SOURCE2	= functions.cpp image.cpp structs.cpp hash_functions.cpp cube.cpp
HEADER2	= functions.h image.h structs.h hash_functions.h
OUT2	= cube

CC		= g++ --std=c++11
FLAGS	 = -g -c -Wall 
LFLAGS	 = 

all: lsh cube

lsh: $(OBJS1) $(LFLAGS)
	$(CC) -g $(OBJS1) -o $(OUT1)

cube: $(OBJS2) $(LFLAGS)
	$(CC) -g $(OBJS2) -o $(OUT2)

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

cube.o: cube.cpp
	$(CC) $(FLAGS) cube.cpp 

clean:
	rm -f $(OBJS1) $(OUT1) $(OBJS2) $(OUT2)

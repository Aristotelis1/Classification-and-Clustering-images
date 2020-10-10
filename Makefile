OBJS	= functions.o image.o man.o structs.o
SOURCE	= functions.cpp image.cpp man.cpp structs.cpp
HEADER	= functions.h image.h structs.h 
OUT	= test
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

function.o: function.cpp function.h
	$(CC) $(FLAGS) function.cpp

image.o: image.cpp image.h
	$(CC) $(FLAGS) image.cpp 

structs.o: structs.cpp structs.h
	$(CC) $(FLAGS) structs.cpp 

man.o: man.cpp
	$(CC) $(FLAGS) man.cpp 


clean:
	rm -f $(OBJS) $(OUT)
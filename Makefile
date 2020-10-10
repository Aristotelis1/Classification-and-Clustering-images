OBJS  =	main.o
SOURCE  =	main.cpp
HEADER  =	image.h	functions.h	struct.h
OUT   =	test
CXX =	g++
FLAGS	=	-std=c++0x	-Wall

all:	$(OBJS)
	$(CXX)	$(OBJS)	-o	$(OUT)	$(FLAGS)
	make	-f	Makefile	clean
  
# create/compile the individual files >>separately<< 
main.o:	main.cpp
	$(CXX)	-c	main.cpp	$(FLAGS)
    
.PHONY:	all
# clean house
clean:
	rm -f $(OBJS)

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)

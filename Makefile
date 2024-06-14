CXXFLAGS=-Wall -std=c++11

all: lzwdr

lzwdr: lzwdr_main.o lzwdr_hashtable.o
	g++ -o lzwdr lzwdr_main.o lzwdr_hashtable.o

lzwdr_main.o: lzwdr_main.cpp lzwdr_biblioteca.h
	g++ -o lzwdr_main.o lzwdr_main.cpp -c $(CXXFLAGS)

lzwdr_hashtable.o: lzwdr_hashtable.cpp lzwdr_biblioteca.h
	g++ -o lzwdr_hashtable.o lzwdr_hashtable.cpp -c $(CXXFLAGS)

clean:
	rm -f *.o
	rm -f lzwdr
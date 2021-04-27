cache-sim: main.o cache.o
	g++ -std=c++11 -o cache-sim main.o cache.o
main.o: main.cpp cache.h
	g++  -std=c++11 -c main.cpp
cache.o: cache.cpp cache.h
	g++ -std=c++11  -c cache.cpp
clean:
	rm cache-sim main.o cache.o

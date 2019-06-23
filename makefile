all: astar gen read

astar: main.cpp astar.cpp
	g++ -std=c++11 -O2 -o astar main.cpp astar.cpp -lpthread

gen: gen.cpp
	g++ -std=c++11 -o gen gen.cpp

read: read.cpp
	g++ -std=c++11 -o read read.cpp

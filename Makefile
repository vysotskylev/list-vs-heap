all: main.cpp cache.h heap.h heap-inl.h
	g++ -O3 -std=c++11 main.cpp -o meas

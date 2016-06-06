CC = g++
SRCDIR = src
CFLAGS = -std=c++14 -fsanitize=address -fopenmp

sampler :
	$(CC) $(CFLAGS) -o bin/sampler $(SRCDIR)/tree.cpp $(SRCDIR)/dist.cpp sampler.cpp

clean :
	\rm bin/*


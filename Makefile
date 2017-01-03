CC = g++
SRCDIR = src
CPPFLAGS = -std=c++14 -fsanitize=address -fopenmp -I./include

sampler :
	$(CC) $(CPPFLAGS) -o bin/sampler $(SRCDIR)/tree.cpp $(SRCDIR)/dist.cpp sampler.cpp

clean :
	\rm bin/*


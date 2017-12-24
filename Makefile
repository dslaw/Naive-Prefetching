CXX = g++
CXXFLAGS = -std=c++14 -fsanitize=address -fopenmp
INCLUDEDIR = include
SRCDIR = src

.PHONY = clean sampler


sampler : $(SRCDIR)/tree.cpp $(SRCDIR)/dist.cpp sampler.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDEDIR) -o bin/$@ $^

clean :
	rm -f bin/*

LDFLAGS = -std=c++11 -stdlib=libc++ -luv

all:
	clang++ -o main.o main.cpp $(LDFLAGS)

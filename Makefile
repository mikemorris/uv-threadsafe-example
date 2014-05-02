LDFLAGS = -std=c++11 -stdlib=libc++ -luv

all:
	clang++ -o main.o main.cpp $(LDFLAGS) --debug

clean:
	rm main.o

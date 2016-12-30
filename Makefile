# g++ -ggdb `pkg-config --cflags --libs /usr/local/Cellar/opencv3/HEAD-7dd3723_4/lib/pkgconfig/opencv.pc` 2.cpp -o 2

SRCS=$(wildcard src/*.cpp)

PROGS=$(patsubst src/%.cpp,bin/%,$(SRCS))

CFLAGS=-std=c++11 -lc++
OPENCV_CFLAGS=$(shell pkg-config --cflags --libs /usr/local/opt/opencv3/lib/pkgconfig/opencv.pc)

all: $(PROGS)

clean:
	rm $(PROGS)

bin/%: src/%.cpp
	g++ $(CFLAGS) $(OPENCV_CFLAGS) -o $@ $<

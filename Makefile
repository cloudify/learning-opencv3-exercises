# g++ -ggdb `pkg-config --cflags --libs /usr/local/Cellar/opencv3/HEAD-7dd3723_4/lib/pkgconfig/opencv.pc` 2.cpp -o 2

SRCS=$(wildcard src/*.cpp)

PROGS=$(patsubst src/%.cpp,bin/%,$(SRCS))

CFLAGS=$(shell pkg-config --cflags --libs /usr/local/lib/pkgconfig/opencv.pc)

all: $(PROGS)

clean:
	rm $(PROGS)

bin/%: src/%.cpp

	g++ $(CFLAGS) -o $@ $<

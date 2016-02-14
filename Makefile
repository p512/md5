CC=g++
CFLAGS=-std=c++14 -O0 -g -Wall -Wextra -Wpedantic
LDFLAGS=
EXEC=md5

.PHONY: all clean

all: $(EXEC)

clean:
	rm -f $(EXEC) *.o

$(EXEC): main.o md5.o
	g++ $^ -o $(EXEC) $(LDFLAGS)

%.o: %.cpp
	g++ -c -o $@ $< $(CFLAGS)  

main.o: main.cpp md5.o
md5.o: md5.hpp md5.cpp

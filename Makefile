CC=g++
CFLAGS=-std=c++14 -Ofast -Wall -Wextra -Wpedantic -flto -DNDEBUG
#CFLAGS=-std=c++14 -O0 -Wall -Wextra -Wpedantic -g
LDFLAGS=-flto
#LDFLAGS=
EXEC=md5

.PHONY: all clean

all: $(EXEC)

clean:
	rm -f $(EXEC) *.o

$(EXEC): main.o md5.o
	$(CC) $^ -o $(EXEC) $(LDFLAGS)

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)  

main.o: main.cpp md5.o
md5.o: md5.hpp md5.cpp

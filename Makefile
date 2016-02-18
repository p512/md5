CC=g++
OPT=-Ofast -flto -march=native -mtune=native
CFLAGS=-std=c++14 -Wall -Wextra -Wpedantic $(OPT) -DNDEBUG
LDFLAGS=$(OPT)
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

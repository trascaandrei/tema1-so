CC = gcc
CFLAGS = -Wall

all: so-cpp

so-cpp: main.o hashmap.o
	$(CC) $(CFLAGS)  $^ -o so-cpp

main.o: main.c
	$(CC) $(CFLAGS) -c $^

hashmap.o: hashmap.c
	$(CC) $(CFLAGS) -c $^

.PHONY:

run:
	./so-cpp
	
clean:
	rm -f *.o so-cpp
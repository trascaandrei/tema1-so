CC = gcc
CFLAGS = -Wall -g

all: so-cpp

so-cpp: main.o hashmap.o helpers.o parser.o
	$(CC) $(CFLAGS)  $^ -o so-cpp

main.o: main.c
	$(CC) $(CFLAGS) -c $^

hashmap.o: hashmap.c
	$(CC) $(CFLAGS) -c $^

helpers.o: helpers.c
	$(CC) $(CFLAGS) -c $^

parser.o: parser.c
	$(CC) $(CFLAGS) -c $^

.PHONY:

run:
	./so-cpp

clean:
	rm -f *.o so-cpp

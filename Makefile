CC = gcc
CFLAGS = -pthread


all:
	$(CC) $(CFLAGS)  prodCon.c -o prodCon
	

clean:
	rm  *prodCon
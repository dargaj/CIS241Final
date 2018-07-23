CC=gcc
CFLAGS=-I .
DEPS = sha1.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

finalHash: sha1.o main.o 
	gcc -o finalHash sha1.o main.o -I .

clean: 
	rm -f *.o main

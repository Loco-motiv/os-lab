CC = gcc
CFLAGS = -Wall -Wextra

main: list_of_free buddy lib main.c
	$(CC) $(CFLAGS) -g -o main.out main.c list_of_free.o buddy.o lib.o

list_of_free: list_of_free.c
	$(CC) $(CFLAGS) -g -c list_of_free.c

buddy: buddy.c
	$(CC) $(CFLAGS) -g -c buddy.c

lib: lib.c
	$(CC) $(CFLAGS) -g -c lib.c

clear:
	rm -f *.out *.o
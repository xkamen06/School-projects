##
 # Predmet: IZP (Zaklady programovani)
 # Projekt: Jendoducha shlukova analyza (clusters)
 # Autor: Zdenek Kamensky 1BIT FIT VUT Brno
 # Datum: 1.12.2015
 ##

PROJ=proj3
PACK=$(PROJ).tar.gz

CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror -pedantic -lm

all: $(PROJ)

$(PROJ): proj3.c 
	$(CC) $(CFLAGS) -o $@ proj3.c

test: $(PROJ)
	./$(PROJ) input_file 4

pack:
	tar -zcvf $(PACK) proj3.c Makefile 

clean:
	rm -f $(PROJ) $(PACK) *.o 

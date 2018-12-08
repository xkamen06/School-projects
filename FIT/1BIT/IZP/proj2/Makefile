##
 # Predmet: IZP (Zaklady programovani)
 # Projekt: Iteracni vypocty - zretezeny zlomek a tayloruv polynom
 #		+ vypocet poctu iteraci pro vysledek s danou presnosti
 # Autor: Zdenek Kamensky 1BIT FIT VUT Brno
 # Datum: 15.11.2015
 ##

PROJ=proj2
PACK=$(PROJ).tar.gz

CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror -pedantic -lm

all: $(PROJ)

$(PROJ): proj2.c 
	$(CC) $(CFLAGS) -o $@ proj2.c

test: $(PROJ)
	./$(PROJ) --log 355 20
	./$(PROJ) --iter 10 100 0.04

pack:
	tar -zcvf $(PACK) proj2.c Makefile 

clean:
	rm -f $(PROJ) $(PACK) *.o 

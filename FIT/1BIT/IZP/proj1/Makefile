##
 # Predmet: IZP (Zaklady programovani)
 # Projekt: Prace s textem - Analyza vstupniho textu (slovo, palindrom, cislo, prvocislo, datum, den v tydnu)
 # Autor: Zdenek Kamensky 1BIT FIT VUT Brno
 # Datum: 25.10.2015
 ##

PROJ=proj1
PACK=$(PROJ).tar.gz

CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror -pedantic

all: $(PROJ)

$(PROJ): proj1.c 
	$(CC) $(CFLAGS) -o $@ proj1.c

test: $(PROJ)
	./$(PROJ) < test_input

pack:
	tar -zcvf $(PACK) proj1.c Makefile 

clean:
	rm -f $(PROJ) $(PACK) *.o 

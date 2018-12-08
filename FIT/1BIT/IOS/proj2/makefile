#
#Nazev souboru: makefile
#Autor: Zdenek Kamensky, 1.rocnik, FIT VUT Brno
#Datum: 27.4.2016
#Popis ukolu: Implementace synchronizacniho problemu Roller Coaster (dva typy procesu - vozik a pasazer - zapisuji do souboru)
#Popis souboru: makefile ke 2. projektu do predmetu IOS
#

CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic 
ENDFLAGS= -lrt -pthread
GXX=gcc

proj2: proj2.o lib.o
	$(GXX) $(CFLAGS) proj2.o lib.o -o proj2 $(ENDFLAGS)

proj2.o: proj2.c lib.h
	$(GXX) $(CFLAGS) -c proj2.c -o proj2.o $(ENDFLAGS)

lib.o: lib.c lib.h
	$(GXX) $(CFLAGS) -c lib.c -o lib.o $(ENDFLAGS)

#Autor: Zdenek Kamensky, FIT VUT Brno
#Predmet: Jazyk C
#Ukol: Makefile k prvnimu DU

#Promenne
CFLAGS=-std=c99 -pedantic -Wall -O2 -lm
CC=gcc
all: steg-decode primes-inline primes 

############VYTVORENI BINARNICH SOUBORU#################################
steg-decode.o: steg-decode.c ppm.h error.h eratosthenes.h bit_array.h
	$(CC) $(CFLAGS) -c steg-decode.c -o steg-decode.o

primes.o: primes.c error.h bit_array.h eratosthenes.h
	$(CC) $(CFLAGS) -c primes.c -o primes.o 

ppm.o: ppm.c ppm.h error.h bit_array.h eratosthenes.h
	$(CC) $(CFLAGS) -c ppm.c -o ppm.o
 
eratosthenes.o: eratosthenes.c eratosthenes.h bit_array.h error.h
	$(CC) $(CFLAGS) -c eratosthenes.c -o eratosthenes.o

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c -o error.o

########################################################################################
			#####SESTAVENI MODULU DO SPOUSTECIHO SOUBORU#####

steg-decode: steg-decode.o ppm.o eratosthenes.o error.o
	$(CC) $(CFLAGS) steg-decode.o eratosthenes.o ppm.o error.o -o steg-decode

primes: primes.o error.o eratosthenes.o 
	$(CC) $(CFLAGS) primes.o error.o eratosthenes.o -o primes

#eratosthenes a primes e znovu skompiluji kvuli pouziti -DUSE_INLINE
primes-inline: primes.c error.o eratosthenes.c 
	$(CC) $(CFLAGS) -DUSE_INLINE primes.c eratosthenes.c error.o -o primes-inline


 	

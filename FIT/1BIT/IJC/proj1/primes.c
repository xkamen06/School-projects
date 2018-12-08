/******* PROGRAM PRO VYPSANI PRVOCISEL OD 2 DO 220 MILIONU *******/
/*
*
* Soubor: primes.c
* Autor: Zdenek Kamensky, 1. rocnik FIT VUT v Brne
* Datum: 20.3.2016
* Predmet: Jazyk C - IJC
* Ukol: Domaci ukol c.1
*
*/

#include <stdio.h>
#include <stdlib.h>
#include "eratosthenes.h" // Eratosthenes()
#include "bit_array.h"  // ba_create() a ba_get_bit() 
#define max 220000000UL

/* Funkce, ktera zjisti poslednich 10 prvocisel v intervalu od 2 do 220 milionu */
static void prvocisla(void)
{
	/* Maximalni cislo intervalu, pocitadlo prvocisel, a limit kolik prvocisel ma byt tisknuto */
	//unsigned long max = 220000000UL;
	unsigned long pocitadlo = 0; 
	unsigned long ma_tisknout = 10;
	
	/* Vytvori se pole o velikosti max, a je inicializovano na nuly */
	ba_create(prms, max); // z bit_array.h
	unsigned long a = max - 1;

	/* Na toto pole se posle Eratosthenovo sito */
	Eratosthenes(prms); // z eratosthenes.h

	/* Cyklus prochazi tento interval od konce a hleda prvocisla dokud jich nenajde 10 - a zustane nastaveno na index,/
	   kde bylo nalezeno posledni prvocislo */
	for(; (a > 0) && (pocitadlo <= ma_tisknout); --a) 
	{
		if(ba_get_bit(prms, a) == 0)
		{	
			pocitadlo++;
		}		
	}

	/* Vypise tato prvocisla tak, ze postupuje opet nahoru od posledniho nalezeno az po horni hranici intervalu */
	for(unsigned long b = a; b < max; ++b)
	{
		if(ba_get_bit(prms, b) == 0)
		{
			printf("%lu\n", b);
		}
	}
}



int main()
{	
	/* Vola se void funkce prvocisla() */
	prvocisla();
	return 0;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!TODO
}

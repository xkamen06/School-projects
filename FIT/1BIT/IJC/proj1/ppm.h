/******* HLAVICKOVY SOUBOR PRO PROGRAM PRO PRACI S PPM SOUBORY *******/
/*
*
* Soubor: ppm.h
* Autor: Zdenek Kamensky, 1. rocnik FIT VUT Brno
* Datum: 20.3.2016
* Predmet: Jazyk C - IJC
* Ukol: Domaci ukol c.1
*
*/

#ifndef PPM
#define PPM 


#include "eratosthenes.h"
#include "bit_array.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>

/* Definice struktury ppm pro uchovani ppm souboruy */
struct ppm 
{ 
	unsigned xsize; 
	unsigned ysize; 
	char data[];
};

/* Prototyp funkce, ktera nacte obsah PPM souboru do touto funkci alokovane struktury ppm */ 
struct ppm * ppm_read(const char * filename);

/* Prototyp funkce, ktera zapise obsah struktury p do souboru ve formátu PPM */
int ppm_write(struct ppm *p, const char * filename);


#endif

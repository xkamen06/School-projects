/******* PROGRAM PRO HLEDANI A VYPIS TAJNE ZPRAVY Z OBRAZKU FORMATU PPM NA STDOUT *******/
/*
*
* Soubor: bit_array.h
* Autor: Zdenek Kamensky, 1. rocnik FIT VUT Brno
* Datum: 20.3.2016
* Predmet: Jazyk C - IJC
* Ukol: Domaci ukol c.1
*
*/


/* Jednostliva rozhrani */
#include "bit_array.h"
#include "eratosthenes.h"
#include "error.h"
#include "ppm.h"


int main(int argc,char * argv[])
{	
	/* Kontrola argumentu programu */
	if(argc < 2)
	{
		warning_msg("Nebyl zadan zadny soubor!\n");
		return -1; 
	}
	if(argc > 2) 
	{
		warning_msg("Prilis mnoho argumentu!\n");
		return -1;
	}
	
	/* Vytvori se struktura do ktere se nactou data ze sobuoru obsazeneho v argv[1] -- argv[0] je nazev programu */
	struct ppm *soubor = ppm_read(argv[1]);
	unsigned long a = soubor->xsize * soubor->ysize * 3;
	unsigned long *pole = (unsigned long *) calloc (a,1); ///pouzito calloc protoze narozdil od malloc setne na nuly  
	
	/* Na prvnim prvku pole je velikost */
	pole[0] = a; 
	
	/* POsle se na pole eratosthenes, ktery nastavi prvociselne bity na prvocislenych mistech na 0 */
	Eratosthenes(pole);

	/* Pocitadlo indexu bitu */
	unsigned long b = 0;

	/* Pomocne pole */
	char pomocna[2] = {'\0'};

	for(unsigned long i = 2; i < a; ++i)
	{
		/* Prochazi se jednotlive bity */
		if(ba_get_bit(pole, i) == 0)
		{
			/* Nastavi se bit pole */
			DU1_SET_BIT_(pomocna, b, (soubor->data[i] & 1) );
            		/* Zvysi se pocitadlo indexu bitu pokud uz neni cislem 7 */
			if (b < (CHAR_BIT)-1)
			{
				++b;
			}
			/* Pokud jeste nejsme na konci, ale uz mame 8 bitu, tiskne se znak a nuluje se pocitadlo */
            		else if (pomocna[1] != '\0')    
               		{
				printf("%c", pomocna[1]);	
				b = 0;
			}
			/*  Jinak konec */
            		else
            		{
                		i = a;
			}
		}
	}
	return 0;
}	

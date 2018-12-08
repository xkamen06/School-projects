/******* IMPLEMENTACE ERATOSTHENOVA SITA *******/		
/*
*
* Soubor: eratosthenes.c
* Autor: Zdenek Kamensky, 1. rocnik FIT VUT Brno
* Datum: 20.3.2016
* Predmet: Jazyk C - IJC
* Ukol: Domaci ukol c.1
*
*/


#include "bit_array.h"
#include <math.h>

/* Eratosthenovo síto je jednoduchý algoritmus pro nalezení všech prvočísel menších než zadaná horní mez */
void Eratosthenes(bit_array_t p)
{
	/* S nulou a jednickou nepracujeme - prvocisla od 2 */
	ba_set_bit(p,0,1);  
    	ba_set_bit(p,1,1);  

	/* Nastime horni mez prochazeni - staci po druhou odmocninu maximalni hodnoty - dalsi vime ze prvocisla nebudou */
    	unsigned long max = (unsigned long) sqrt(p[0]);  
    	unsigned long j = 1;
        
	/* Prochazi cisla od dvojky po max a vsechny nasobky techto cisel nastavuje na 1 - nejsou prvocisla */ 
   	for (unsigned long i = 2; i <= max; ++i)
   	{
        	if (ba_get_bit(p,i) == 0)       
        	{
            		while ( j*i < p[0])    
           		{
                		ba_set_bit(p, j*i, 1);
               			++j;
           		}
           		j = i;
      	  	}
   	}
    	ba_set_bit(p,2,0);      
}

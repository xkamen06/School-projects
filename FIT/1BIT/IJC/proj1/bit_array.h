/******* HLAVICKOVY SOUBOR PRO DEFINICI POLE BITU, MAKER A INLINE FUNKCI *******/
/*
*
* Soubor: bit_array.h
* Autor: Zdenek Kamensky, 1. rocnik FIT VUT Brno
* Datum: 20.3.2016
* Predmet: Jazyk C - IJC
* Ukol: Domaci ukol c.1
*
*/

#include <limits.h> ///CHAR_BIT
#include "error.h"

/* PODMINENY PREKLAD - POKUD JIZ NENI DEFINOVANO, DEFINUJE SE */
#ifndef BIT_ARRAY
#define BIT_ARRAY



/* Typedef bitove pole */
typedef unsigned long bit_array_t[];


/*** ------MAKRA------ ***/

/* Pocet unsigned longu pro pozadovanou velikost bitoveho pole  */
#define bit_arr(arr_size)	( (arr_size) % (CHAR_BIT*sizeof(unsigned long)) == 0 ? (arr_size) / (CHAR_BIT*sizeof(unsigned long)) : (arr_size) / (CHAR_BIT*sizeof(unsigned long)) +1 )
	
/* Vytvori bitove pole o danem nazvu a velikosti, na 0 idx pole bude jeho velikost  */
#define ba_create(arr_name, arr_size)	unsigned long arr_name[bit_arr(arr_size) + 1] = {(arr_size), 0} 	

/* Vrati hodnotu bitu v zadanem poli na zadanem indexu */
#define DU1_GET_BIT_(p, i)	( (p[1 + ((i) / ( sizeof(unsigned long)*CHAR_BIT ))] & (1UL << ((i) % ( sizeof(unsigned long)*CHAR_BIT )))) > 0UL ? 1 : 0 )  

/* Nastavi hodnotu bitu v zadanem poli na zadanem indexu na zadanou hodnotu */
#define DU1_SET_BIT_(p, i, b)	( (b) == 0 ? (p[1 + ((i) / ( sizeof(unsigned long)*CHAR_BIT ))] &= ~(1UL << ((i) % ( sizeof(unsigned long)*CHAR_BIT )))) \
						    : (p[1 + ((i) / ( sizeof(unsigned long)*CHAR_BIT ))] |= 1UL << (i) % ( sizeof(unsigned long)*CHAR_BIT )) )


/* PODMINENY PREKLAD - POKUD NENI DANO USE_LINE, DEFINUJI SE MAKRA */
#ifndef USE_LINE



/* Vrati velikost pole v bitech */
#define ba_size(arr_name)	( arr_name[0] )
	
/* Zkontroluje zda pozadovany index jiz neni za mezi pole, pokud je, vypise chybu, pokud ne, tak makro DU1_SET_BIT_ */
#define ba_set_bit(arr_name, idx, exp) 	( (idx) > (arr_name[0]) ? fatal_error("Index %ld mimo rozsah 0..%ld", idx, arr_name[0] - 1),0 : (DU1_SET_BIT_((arr_name), (idx), (exp))) )

/* Zkontroluje zda pozadovany index jiz neni za mezi pole, pokud je, vypise chybu, pokud ne, tak makto DU1_GET_BIT_ */ 
#define ba_get_bit(arr_name, idx) 	( (idx) > (arr_name[0]) ? fatal_error("Index %ld mimo rozsah 0..%ld", idx, arr_name[0] - 1),0 : (DU1_GET_BIT_((arr_name), (idx))) )



/* POKUD JE DEFINOVANO USE_LINE, DEFINUJI SE INLINE FUNKCE */
#else



/*** ------INLINE FUNKCE------ ***/

/* Nastavi hodnotu bitu daneho indexu na zadany vyraz */ 
inline void ba_set_bit (bit_array_t arr, unsigned long idx, int exp)
{
	if(idx >= arr[0])
	{
		fatal_error("Index %ld mimo rozsah 0..%ld", idx, (arr_name[0] - 1))
	}
	else
	{
		DU1_SET_BIT_(arr, idx, exp);
	} 
}

/* Vrati hodnotu bitu daneho indexu bitoveho pole */ 
inline int ba_get_bit (bit_array_t arr, unsigned long idx)
{
	if(idx >= arr[0])
	{
		fatal_error("Index %ld mimo rozsah 0..%ld", idx, (arr_name[0] - 1))
	}
	else 
	{
		DU1_GET_BIT_(arr, idx);
	}
}

inline unsigned int ba_size (bit_array_t arr)
{
	return ( arr[0] );
}


/* KONEC #DEFINE USE_LINE */
#endif


/* KONEC #DEFINE BIT_ARRAYH */ 
#endif 

/******* IMPLEMENTACE VYPISU CHYB NA STDERR POUZITA V OSTATNICH MODULECH *******/
/*
*
* Soubor: error.c
* Autor: Zdenek Kamensky, 1. rocnik FIT VUT Brno
* Datum: 20.3.2016
* Predmet: Jazyk C - IJC
* Ukol: Domaci ukol c.1
*
*/

#include "error.h" ///prototypy

/* Funkce vypise chybove hlaseni na stderr , dostava formatovaci retezec (const char *...) aruzny pocet dalsich parametru */
/* Po vypisu chyby ukonci program s navratovou hodnotou */
void fatal_error(const char *fmt, ...)
{
	fprintf(stderr, "CHYBA: ");
	
	va_list arg;
	va_start(arg, fmt);
	vfprintf(stderr, fmt, arg);
	va_end(arg);

	exit(1);
}


/* Stejne jako predchozi funkce s tim rozdilem, ze neukonci program */
void warning_msg(const char *fmt, ...)
{
   	fprintf(stderr,"CHYBA: ");
    
	va_list arg;       
	va_start(arg, fmt);
	vfprintf(stderr, fmt, arg);
	va_end(arg);
}




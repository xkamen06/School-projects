/******* HLAVICKOVY SOUBOR S PROTOTYPY FUNKCI FATAL_ERROR A WARNING_MSG *******/
/*
*
* Soubor: error.h
* Autor: Zdenek Kamensky, 1. rocnik FIT VUT Brno
* Datum: 20.3.2016
* Predmet: Jazyk C - IJC
* Ukol: Domaci ukol c.1
*
*/

#ifndef ERROR
#define ERROR

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/* Prototyp funkce warning_msg */
void warning_msg(const char *fmt, ...);

/* Prototyp funkce fatal_error */
void fatal_error(const char *fmt, ...);


#endif

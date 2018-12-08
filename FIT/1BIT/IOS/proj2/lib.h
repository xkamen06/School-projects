/*
*
*Nazev souboru: lib.h
*Autor: Zdenek Kamensky, 1.rocnik, FIT VUT Brno
*Datum: 27.4.2016
*Popis ukolu: Implementace synchronizacniho problemu Roller Coaster (dva typy procesu - vozik a pasazer - zapisuji do souboru)
*Popis souboru: Rozhrani pro prototypy funkci a includovani potrebnych knihoven
*
*/

#ifndef	LIB_H
#define LIB_H

//potrebne knihovny
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <errno.h> 
#include <semaphore.h>
#include <fcntl.h>

/***** Prototyp funkce pro prevedeni argumentu (pole charu) do celeho cisla (integeru) *****/ 
int arg_to_int(char arg[]);

/***** Prototyp funkce pro vozik *****/
void car_f(int passengers, int capacity, int max_sleep, int *shared_memory, sem_t *counter, sem_t *load, sem_t *next, sem_t *unboard, sem_t *next2, sem_t *finish, FILE *soubor);

/***** Prototyp funkce pro pasazera *****/
void passenger_f(int *shared_memory, sem_t *counter, sem_t *pass, sem_t *load, sem_t *next, sem_t *unboard, sem_t *next2, sem_t *finish, sem_t *move, int capacity, FILE *soubor);

#endif //LIB_H


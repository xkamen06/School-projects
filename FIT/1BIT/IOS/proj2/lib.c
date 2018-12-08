/*
*
*Nazev souboru: lib.c
*Autor: Zdenek Kamensky, 1.rocnik, FIT VUT Brno
*Datum: 27.4.2016
*Popis ukolu: Implementace synchronizacniho problemu Roller Coaster (dva typy procesu - vozik a pasazer - zapisuji do souboru)
*Popis souboru: Implementace funkci pro vozik, pasazery, ale take pro prevedeni argumentu na datovy typ integer a mocninna funkce
*
*/


#include "lib.h" //zde se nachazi prototypy funkci include a vsech potrebnych knihoven


/***** Funkce pro umocneni cisla 10 na expoenent exp *****/
int pow_f(int exp)
{
	int result = 1;
	
	int a;	
	for(a = 0; a < exp; a++)
		result *= 10;
	
	return result; 
} //pow_f()



/***** Funkce pro prevedni argumentu (pole charu) do celeho cislo (integeru) *****/
int arg_to_int(char arg[])
{
	int num_value; //ciselna hodnota znaku a pozdeji cislo
	int result = 0; //vysledna ciselna hodnota 
	int counter = 0; //pocitadlo znaku v retezci
		
	//pocita znaky v retezci po konec retezce
	while(arg[counter] != '\0')
		counter++;
	
	int i = 0;
	while(arg[i] != '\0')
	{
		num_value = arg[i]; //hodnota znaku v ASCII

		//pokud to neni hodnota reprezentujici cislo - error
		if(num_value < 48 || num_value > 57)
		{
			return -1;	//ERROR
		}

		num_value -= 48; //realna cislena hodnota
		num_value = num_value * pow_f(counter - 1); //celkova hodnota podle radu 
		result += num_value; // pricte k vyslednemu cislu
		
		counter--;
		i++;
	}

	return result;
} //arg_to_int()


/***** Funkce pro implementaci ukonu voziku *****/
void car_f(int passengers, int capacity, int max_sleep, int *shared_memory, sem_t *counter, sem_t *load, sem_t *next, sem_t *unboard, sem_t *next2, sem_t *finish, FILE *soubor)
{
	//semafor se uzamkne, aby mohl proces zapsat do sdilene pameti cislo akce 
	sem_wait(counter);
	fprintf(soubor, "%d\t: C 1\t: started\n", shared_memory[0]++); //startuje
	sem_post(counter);
	
	int i;
	int j;
	int k;
	int l;
	int m;
	//opakuje se po pocet nutnych jizd
	for(i = 0; i < (passengers / capacity); i++)
	{
		//semafor se uzamkne, aby mohl proces zapsat do sdilene pameti cislo akce 
		sem_wait(counter);
		fprintf(soubor, "%d\t: C 1\t: load\n", shared_memory[0]++); //loaduje
		sem_post(counter);

		//odemkne semafory pro pasazery - tim oni zahaji svoji cinnost -- naboarduji se
		for(j = 0; j < capacity; j++)
		{
			sem_post(load);
		}

		//ceka na vsechny pasazery, kteri nastupuji
		for(k = 0; k < capacity; k++)
		{
			sem_wait(next);
		}

		//semafor se uzamkne, aby mohl proces zapsat do sdilene pameti cislo akce 
		sem_wait(counter);
		fprintf(soubor, "%d\t: C 1\t: run\n", shared_memory[0]++); //vyrazi na trat
		sem_post(counter);
		
		//usne na random cas(v milisekundach - usleep je v mikrosekundach proto *1000), avsak maximelne na zadany cas
		if(max_sleep > 0)
		{
			usleep(rand() % (max_sleep * 1000));
		}
		
		//semafor se uzamkne, aby mohl proces zapsat do sdilene pameti cislo akce 
		sem_wait(counter);
		fprintf(soubor, "%d\t: C 1\t: unload\n", shared_memory[0]++); //unloaduje
		sem_post(counter);

		//odemkne semafory pro pasazery - tim oni zahaji svoji cinnost - zacnou vystupovat
		for(l = 0; l < capacity; l++)
		{
			sem_post(unboard);
		}

		//ceka na vystoupeni vsech pasazeru kteri jeli
		for(m = 0; m < capacity; m++)
		{
			sem_wait(next2);
		}

	}


	//semafor se uzamkne, aby mohl proces zapsat do sdilene pameti cislo akce  
	sem_wait(counter);
	fprintf(soubor, "%d\t: C 1\t: finished\n", shared_memory[0]++); //ukoncuje se
	sem_post(counter);

	//odemkne semafor aby pasazeri vedeli, ze uz je konec
	int a;
	for(a = 0; a < passengers; a++)
	{
		sem_post(finish);
	}
} //car_f()



/***** Implementace funkce pasazera a vsech jeho ukonu *****/
void passenger_f(int *shared_memory, sem_t *counter, sem_t *pass, sem_t *load, sem_t *next, sem_t *unboard, sem_t *next2, sem_t *finish, sem_t *move, int capacity, FILE *soubor)
{
	//ulozi si do promenne jake ma cislo a toho cislo ve sdilene pameti inkrementuje - cte a zapisuje do sdilene pameti - musi byt pouzit semafor
	sem_wait(pass);
	int ID = shared_memory[1]++;
	sem_post(pass);

	//semafor se uzamkne, aby mohl proces zapsat do sdilene pameti cislo akce
	sem_wait(counter);
	fprintf(soubor, "%d\t: P %d\t: started\n", shared_memory[0]++, ID); //startuje
	sem_post(counter);
	
	//ceka za dostane pokyn od procesu vozik aby mohl zacit nastupovat
	sem_wait(load);

	//semafor pro poradi nastupovani do voziku
	sem_wait(move);

	//pokud je poslednim vystupujicim
	if(shared_memory[2] == (capacity - 1))
	{
		//semafor se uzamkne, aby mohl proces zapsat do sdilene pameti cislo akce
		sem_wait(counter);
		fprintf(soubor, "%d\t: P %d\t: board\n", shared_memory[0]++, ID); //nastupuje
		fprintf(soubor, "%d\t: P %d\t: board last\n", shared_memory[0]++, ID); //nastupuje jako posledni
		sem_post(counter);
		
		//zvysi pocet nastoupenych ve sdilene pameti - pro toto slouzi semafor move
		shared_memory[2]++;
	}
	else //jinak pokud neni poslednim vystupujicim
	{
		//semafor se uzamkne, aby mohl proces zapsat do sdilene pameti cislo akce
		sem_wait(counter);
		fprintf(soubor, "%d\t: P %d\t: board\n", shared_memory[0]++, ID); //nastupuje
		fprintf(soubor, "%d\t: P %d\t: board order %d\n", shared_memory[0]++, ID, ++shared_memory[2]); //nastupuje s poradovym cislem....
		sem_post(counter);
	
	}

	//odemkne semafor pro sdilenou pamet [2]
	sem_post(move); 



	//oznami voziku ze nastoupil
	sem_post(next);
	
	//ceka na pokyn od procesu vozik, aby mohl vystoupit 
	sem_wait(unboard);

	//semafor pro poradi nastupovani do voziku
	sem_wait(move);
	//kdyz je poslednim vystupujicim 
	if(shared_memory[2] == 1)
	{
		//semafor se uzamkne, aby mohl proces zapsat do sdilene pameti cislo akce
		sem_wait(counter);
		fprintf(soubor, "%d\t: P %d\t: unboard\n", shared_memory[0]++, ID); //vystupuje
		fprintf(soubor, "%d\t: P %d\t: unboard last\n", shared_memory[0]++, ID); //vystupuje posledni
		sem_post(counter);
		
		//dekrementuje pocet pasazeru, kteri jeste jsou ve voziku
		shared_memory[2]--;	
	}
	else
	{
		//semafor se uzamkne, aby mohl proces zapsat do sdilene pameti cislo akce
		sem_wait(counter);
		fprintf(soubor, "%d\t: P %d\t: unboard\n", shared_memory[0]++, ID); //vystupuje
		fprintf(soubor, "%d\t: P %d\t: unboard order %d\n", shared_memory[0]++, ID, ((capacity+1) - shared_memory[2])); //vystupuje s poradovym cislem....
		sem_post(counter);
		

		//dekrementuje pocet pasazeru, kteri jeste jsou ve voziku
		shared_memory[2]--;
	}

	//odemkne semafor pro sdilenou pamet [2]
	sem_post(move); 

	

	//oznami voziku ze uz vystoupil
	sem_post(next2);
	
	//ceka na to az bude moci skoncit
	sem_wait(finish);

	//semafor se uzamkne, aby mohl proces zapsat do sdilene pameti cislo akce	
	sem_wait(counter);
	fprintf(soubor, "%d\t: P %d\t: finished\n", shared_memory[0]++, ID); //konci
	sem_post(counter);
} //passenger_f()
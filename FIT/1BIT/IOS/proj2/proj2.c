/*
*
*Nazev souboru: proj2.c
*Autor: Zdenek Kamensky, 1.rocnik, FIT VUT Brno
*Datum: 27.4.2016
*Popis ukolu: Implementace synchronizacniho problemu Roller Coaster (dva typy procesu - vozik a pasazer - zapisuji do souboru)
*Popis souboru: ..................................................
*
*/


#include "lib.h" //v tomto rozhrani jsou vsechny nutne knihovny a prototypy funkci


/*********** MAKRA ***********/
#define MEMKEY "/xkamen06_mem"
#define FILENAME "proj2.out"


//////////////////////////////osetrit v lib.c v arg_to_int preteceni intu <----- asi je osetreno////////////////////////////
//////////////////////////////osetrit spatne ukonceni programu ---- budou problemy s tim kdyz se spatne forkne atd./////////


int main(int argc, char *argv[])
{ 

	setbuf(stderr, NULL); //buffer chyboveho vystupu nastavi na NULL

	/******************************************* OSETRENI VSTUPNICH ARGUMENTU **********************************************/

	if(argc != 5) //neodpovidajici pocet argumentu
	{	
		fprintf(stderr, "ERROR: Invalid number of arguments.\n");
		exit(1);
	}

	int param_1 = arg_to_int(argv[1]); //vsechny argumenty se prevedou na dat. typ integer pomoci fce z vytvoreneho rozhrani
	int param_2 = arg_to_int(argv[2]); // ....
	int param_3 = arg_to_int(argv[3]); // ....
	int param_4 = arg_to_int(argv[4]); // ....

	//argumenty neodpovidaji pozadovanym intervalum  
	if((param_1 <= 0) || (param_2 <= 0) || ((param_3 < 0) || (param_3 > 5000)) || ((param_4 < 0) || (param_4 > 5000)) )
	{
		fprintf(stderr, "ERROR: Invalid arguments.\n");
		exit(1);
	}

	//pocet pasazeru neni delitelny kapacitou voziku 
	if((param_1 <= param_2) || ((param_1 % param_2) != 0))
	{
		fprintf(stderr, "ERROR: Invalid arguments!\n");
		exit(1);
	}

	/*********************************************************************************************************************/

	/************** VYTVORENI SDILENE PAMETI, NAALOKOVANI POLE PRO ID PASAZERU A OTEVRENI SOUBORU PRO VYSTUP **************/

	FILE *soubor;
	soubor = fopen(FILENAME, "w+"); //soubor je otevren pro zapis i cteni a pokud neni vytvoren, vytvori se 
	if(soubor == NULL) //soubor se nepodarilo otevrit/vytvorit
	{
		fprintf(stderr, "ERROR: Can't open output file.\n");
		exit(2);
	}
	setbuf(soubor, NULL); //nastaveni bufferu pro vystupni soubor na NULL
	
	//vytvoreni objektu ve sdilene pameti 
	int mem_obj = shm_open(MEMKEY, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	//kontrola uspesnosti vytvoreni
	if(mem_obj == -1)
	{
		fprintf(stderr, "ERROR: Can't alloc a shared memory.\n");
		fclose(soubor);
		exit(2);
	}

	//oriznuti pameti na velikost 3 * sizeof(int) - vetsinou 12 byte na rozumnych architekturach
	int contr_trunc = ftruncate(mem_obj, 3*sizeof(int));
	//kotnrola uspesnosti osekani
	if(contr_trunc == -1)
	{
		fprintf(stderr, "ERROR: Can't truncate size of object in shared memory.\n"); 
		close(mem_obj);
		fclose(soubor);
		exit(2);
	}
	
	//namapovani tri bloku do sdilene pameti na miste, kde byla vytvorena a osekana + inicializovano na 0
	int *mem_blocks;
	mem_blocks = (int *)mmap(NULL, 3*sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, mem_obj, 0);
    //kotnrola uspesnosti mapovani
	if(mem_blocks == MAP_FAILED)
    {
		fprintf(stderr, "ERROR: Can't divide the memory to blocks.\n");
		close(mem_obj);
		fclose(soubor);
		exit(2);
	}
	
	/***** SDILENA PAMET *****/
	mem_blocks[0] = 1; //pocitadlo akci
	mem_blocks[1] = 1; //pocitadlo pasazeru
	mem_blocks[2] = 0; //pocitadlo pasazeru, kteri jsou ve voziku


	//alokovani pameti pro ID pasazeru a nasledne overeni uspesnosti alokace
	pid_t *passenger_ID = malloc(param_1*sizeof(pid_t));
	if(passenger_ID == NULL)
	{
		fprintf(stderr, "%s\n", "ERROR: Can't alloc memory for passenger's IDs.\n");
		close(mem_obj);
		fclose(soubor);
		munmap(mem_blocks, 3*sizeof(int));
		exit(2);
	}
	
	/********************************************************************************************************************/

	/***************************************************** SEMAFORY *****************************************************/
	
	sem_t *counter; //pro zapis do sdilene pameti pro index akci
	sem_t *load; //pro oznameni pasazerum vozikem ze jiz mohou nastupovat
	sem_t *pass; //pro zapis do sdilene pameti pro index pasazeru
	sem_t *next; //
	sem_t *unboard; //pro oznameni pasazerum vozikem ze jiz mohou vystupovat
	sem_t *next2; //
	sem_t *finish; //
	sem_t *move; //pro pristup do sdilene pameti pro pocitani kolikaty je vystupujici ci nastupujici dany pasazer

	//vytvoreni vsech semaforu a kontrola spravnosti vytvoreni

	if((move  = sem_open("/xkamen_06_move", O_CREAT|O_EXCL, S_IRUSR|S_IWUSR, 1)) == SEM_FAILED)
	{
		fprintf(stderr, "%s\n", "ERROR: Can't generate semaphore.");
		sem_close(move);
		sem_unlink("/xkamen_06_move");
		close(mem_obj);
		fclose(soubor);
		munmap(mem_blocks, 3*sizeof(int));
		free(passenger_ID);
		exit(2);
	}

	if((unboard  = sem_open("/xkamen_06_unboard", O_CREAT|O_EXCL, S_IRUSR|S_IWUSR, 0)) == SEM_FAILED)
	{
		fprintf(stderr, "%s\n", "ERROR: Can't generate semaphore.");
		sem_close(unboard);
		sem_close(move);
		sem_unlink("/xkamen_06_unboard");
		sem_unlink("/xkamen_06_move");
		close(mem_obj);
		fclose(soubor);
		munmap(mem_blocks, 3*sizeof(int));
		free(passenger_ID);
		exit(2);
	}

	if((finish  = sem_open("/xkamen_06_finish", O_CREAT|O_EXCL, S_IRUSR|S_IWUSR, 0)) == SEM_FAILED)
	{
		fprintf(stderr, "%s\n", "ERROR: Can't generate semaphore.");
		sem_close(finish);
		sem_close(move);
		sem_close(unboard);
		sem_unlink("/xkamen_06_finish");
		sem_unlink("/xkamen_06_move");
		sem_unlink("/xkamen_06_unboard");
		close(mem_obj);
		fclose(soubor);
		munmap(mem_blocks, 3*sizeof(int));
		free(passenger_ID);
		exit(2);
	}

	if((next2  = sem_open("/xkamen_06_next2", O_CREAT|O_EXCL, S_IRUSR|S_IWUSR, 0)) == SEM_FAILED)
	{
		fprintf(stderr, "%s\n", "ERROR: Can't generate semaphore.");
		sem_close(next2);
		sem_close(move);
		sem_close(unboard);
		sem_close(finish);
		sem_unlink("/xkamen_06_next2");
		sem_unlink("/xkamen_06_move");
		sem_unlink("/xkamen_06_unboard");
		sem_unlink("/xkamen_06_finish");
		close(mem_obj);
		fclose(soubor);
		munmap(mem_blocks, 3*sizeof(int));
		free(passenger_ID);
		exit(2);
	}

	if((next  = sem_open("/xkamen_06_next", O_CREAT|O_EXCL, S_IRUSR|S_IWUSR, 0)) == SEM_FAILED)
	{
		fprintf(stderr, "%s\n", "ERROR: Can't generate semaphore.");
		sem_close(next);
		sem_close(move);
		sem_close(unboard);
		sem_close(finish);
		sem_close(next2);
		sem_unlink("/xkamen_06_next");
		sem_unlink("/xkamen_06_move");
		sem_unlink("/xkamen_06_unboard");
		sem_unlink("/xkamen_06_finish");
		sem_unlink("/xkamen_06_next2");
		close(mem_obj);
		fclose(soubor);
		munmap(mem_blocks, 3*sizeof(int));
		free(passenger_ID);
		exit(2);
	}

	if((counter  = sem_open("/xkamen_06_counter", O_CREAT|O_EXCL, S_IRUSR|S_IWUSR, 1)) == SEM_FAILED)
	{
		fprintf(stderr, "%s\n", "ERROR: Can't generate semaphore.");
		sem_close(counter);
		sem_close(move);
		sem_close(unboard);
		sem_close(finish);
		sem_close(next2);
		sem_close(next);
		sem_unlink("/xkamen_06_counter");
		sem_unlink("/xkamen_06_move");
		sem_unlink("/xkamen_06_unboard");
		sem_unlink("/xkamen_06_finish");
		sem_unlink("/xkamen_06_next2");
		sem_unlink("/xkamen_06_next");
		close(mem_obj);
		fclose(soubor);
		munmap(mem_blocks, 3*sizeof(int));
		free(passenger_ID);
		exit(2);
	}

	if((load = sem_open("/xkamen_06_load", O_CREAT|O_EXCL, S_IRUSR|S_IWUSR, 0)) == SEM_FAILED)
	{
		fprintf(stderr, "%s\n", "ERROR: Can't generate semaphore.");
		sem_close(load);
		sem_close(move);
		sem_close(unboard);
		sem_close(finish);
		sem_close(next2);
		sem_close(next);
		sem_close(counter);
		sem_unlink("/xkamen_06_load");
		sem_unlink("/xkamen_06_move");
		sem_unlink("/xkamen_06_unboard");
		sem_unlink("/xkamen_06_finish");
		sem_unlink("/xkamen_06_next2");
		sem_unlink("/xkamen_06_next");
		sem_unlink("/xkamen_06_counter");
		close(mem_obj);
		fclose(soubor);
		munmap(mem_blocks, 3*sizeof(int));
		free(passenger_ID);
		exit(2);
	}

	if((pass  = sem_open("/xkamen_06_pass", O_CREAT|O_EXCL, S_IRUSR|S_IWUSR, 1)) == SEM_FAILED)
	{
		fprintf(stderr, "%s\n", "ERROR: Can't generate semaphore.");
		sem_close(pass);
		sem_close(move);
		sem_close(unboard);
		sem_close(finish);
		sem_close(next2);
		sem_close(next);
		sem_close(counter);
		sem_close(load);
		sem_unlink("/xkamen_06_pass");
		sem_unlink("/xkamen_06_move");
		sem_unlink("/xkamen_06_unboard");
		sem_unlink("/xkamen_06_finish");
		sem_unlink("/xkamen_06_next2");
		sem_unlink("/xkamen_06_next");
		sem_unlink("/xkamen_06_counter");
		sem_unlink("/xkamen_06_load");
		close(mem_obj);
		fclose(soubor);
		munmap(mem_blocks, 3*sizeof(int));
		free(passenger_ID);
		exit(2);
	}

	/***********************************************************************************************************************/

	int control = 0;
	pid_t car;

	//vytvori se vedlejsi proces a do promenne se ulozi jeho cislo a overi se uspesnost tohoto generovani
	pid_t minor_process = fork();
	if(minor_process < 0)
	{
		fprintf(stderr, "%s\n", "ERROR: Can't generate new process.");
		sem_close(pass);
		sem_close(move);
		sem_close(unboard);
		sem_close(finish);
		sem_close(next2);
		sem_close(next);
		sem_close(counter);
		sem_close(load);
		sem_unlink("/xkamen_06_pass");
		sem_unlink("/xkamen_06_move");
		sem_unlink("/xkamen_06_unboard");
		sem_unlink("/xkamen_06_finish");
		sem_unlink("/xkamen_06_next2");
		sem_unlink("/xkamen_06_next");
		sem_unlink("/xkamen_06_counter");
		sem_unlink("/xkamen_06_load");
		close(mem_obj);
		fclose(soubor);
		munmap(mem_blocks, 3*sizeof(int));
		free(passenger_ID);
		exit(2);
	}
	//pokud probiha aktualni proces
	if(minor_process == 0)
	{
		
		//vygenerovani procesu voziku a nasledna kontrola usesnosti generovani
		car = fork();
		if(car < 0)
		{
			fprintf(stderr, "%s\n", "ERROR: Can't generate new process.");
			exit(2);
		}
		//pokud probiha proces voziku
		if(car == 0)
		{
			car_f(param_1, param_2, param_4, mem_blocks, counter, load, next, unboard, next2, finish, soubor); //funkce pro vozik z lib.h
			exit(0);
		}
		
		//generovani procesu pasazeru
		int a;
		for(a = 0; a < param_1; a++)
		{
			//generuje dalsiho pasazera a kotroluje ospesnost generovani
			pid_t passenger = fork();
			if(passenger < 0)
			{
				fprintf(stderr, "%s\n", "ERROR: Can't generate new process.");
				exit(2);
			}
			//pokud jsem v procesu pasazera
			else if(passenger == 0)
			{
				passenger_f(mem_blocks, counter, pass, load, next, unboard, next2, finish, move, param_2, soubor); //funkce pro pasazera z lib.h
				exit(0);
			}
			else
			{
				//ukladani identifikatoru procesu pasazeru do pole
				passenger_ID[a] = passenger;
			}
			if(param_3 != 0)
			{
				usleep(rand() % (param_3 * 1000)); //nahodna doba ovsem max po param_3, po kterou se proces uspi nez generuje dalsi pasazery	
			}
		}
		int car_checker;
		waitpid(car, &car_checker, 0);
		if((WEXITSTATUS(car_checker)) != 0)
		{
			control = 2;
		}
		//pocka se na vsechny procesy pasazer dokud neskonci
		int i;
		int pass_checker;
		for(i = 0; i < param_1; i++)
		{
			waitpid(passenger_ID[i], &pass_checker, 0);
			if((WEXITSTATUS(pass_checker)) != 0)
			{
				control = 2;
			}
		}
		
		exit(0);
	} 
	else
	{
		int minor_checker;
		//pocka se na pomocny proces
		waitpid(minor_process, &minor_checker, 0);
		if((WEXITSTATUS(minor_checker)) != 0)
		{
			control = 2;
		}
	}

	//uvolni se vyuzite zdroje
	free(passenger_ID);
	close(mem_obj);
	munmap(mem_blocks, 3*sizeof(int));
	fclose(soubor);
	sem_close(counter);
	sem_unlink("/xkamen_06_counter");
	sem_close(finish);
	sem_unlink("/xkamen_06_finish");
	sem_close(load);
	sem_unlink("/xkamen_06_load");
	sem_close(pass);
	sem_unlink("/xkamen_06_pass");	
	sem_close(next);
	sem_unlink("/xkamen_06_next");	
	sem_close(unboard);
	sem_unlink("/xkamen_06_unboard");
	sem_close(next2);
	sem_unlink("/xkamen_06_next2");
	sem_close(move);
	sem_unlink("/xkamen_06_move");

	if(control == 2)
	{
		return 2;
	}
	
	return 0;
}//int main()
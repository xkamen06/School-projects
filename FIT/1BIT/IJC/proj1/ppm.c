/******* IMPLEMENTACE PRO PRACI S PPM SOUBORY *******/
/*
*
* Soubor: ppm.c
* Autor: Zdenek Kamensky, 1. rocnik FIT VUT Brno
* Datum: 20.3.2016
* Predmet: Jazyk C - IJC
* Ukol: Domaci ukol c.1
*
*/

#include "ppm.h" ///rozhrani teto implementace

/* Funkce pro zapis dat ze struktury do souboru */ 
int ppm_write(struct ppm *p, const char * filename)
{
	/* Otevreni respektive vytvoreni souboru a test uspesnosti */
	FILE *file_ppm = fopen(filename, "wb");
	if(file_ppm == NULL)
	{
		warning_msg("Chyba pri otevirani souboru!\n");
		return -1;
	}
	/* Vytvoreni prvnich tri radku souboru */
	fprintf(file_ppm, "P6\n%d %d\n255\n",  p->xsize, p->ysize);
	int i = fwrite(p->data, 1, (p->xsize * p->ysize) * 3, file_ppm);
    	if (i < ((p->xsize * p->ysize) * 3) )
    	{
		warning_msg("Chyba pri zapisovani dat.\n");
		if (fclose(file_ppm) == EOF)
		{
			warning_msg("Soubor se nepodarilo uzavrit\n");
		}
              	return -1;
    	}
	/* SOubor je uzavren nebo pokud se jej nepovedlo uzavrit je vypsana chyba */
    	if (fclose(file_ppm) == EOF)
	{
		warning_msg("Soubor se nepodarilo uzavrit\n");
		return -1; 
	}
    	return 0;
}


/* Funkce pro cteni dat ze soboru a ukladani do struktury */
struct ppm * ppm_read(const char * filename)
{
	/* Pomocne promenne pro praci se strukturou a souborem */
	int size_ppm;
	unsigned long x_size;
	unsigned long y_size;
	
	/* Otevreni souboru a test na uspesnost tohoto otevreni */
	FILE *file_ppm = fopen(filename, "rb");
	if(file_ppm == NULL)
	{
		warning_msg("Pri otevirani zadaneho souboru nastala chyba!\n");
		return NULL;
	}

	/* Nacteni casti do pomocnych promennych a overeni uspesnosti cteni a formatu souboru */
	int a = fscanf(file_ppm, "P6 %lu %lu %d%*c", &x_size, &y_size,  &size_ppm);
	if(a != 3 || size_ppm != 255)
	{
		if(fclose(file_ppm) == EOF)
		{
			warning_msg("Soubor se nepodarilo uzavrit\n");
		}
		warning_msg("Tento soubor neni formatu ppm!\n");
		return NULL;
	}
	
	/* Dynamicka alokace pameti pro strukturu typu ppm a overeni spravnosti alokace */
	struct ppm * f_ppm = malloc(sizeof(struct ppm) + x_size*y_size*3);
	if(f_ppm == NULL)
	{
		if(fclose(file_ppm) == EOF)
		{	
			warning_msg("Soubor se nepodarilo uzavrit\n");

		}
		warning_msg("Neuspesna alokace mista v pameti!\n");
		return NULL;
	}
	
	/* Pokud byl soubor otevren, nacten a bylo alokovano misto v pameti, pak jsou hodnoty z pomocnych promennych zavedeny do struktury typu ppm */
	f_ppm->xsize = x_size;
	f_ppm->ysize = y_size;
	
	/* Do promenne data[] ve strukture typu ppm se ulozi jednotlive RGB hodnoty a otestuje se spravnost tohoto nacteni */
	int b = fread(f_ppm->data, 1, x_size*y_size*3, file_ppm);
	if(b < x_size*y_size*3)
	{
		if(fclose(file_ppm) == EOF)
		{
			warning_msg("Soubor se nepodarilo uzavrit\n");
		}
		free(f_ppm);
		warning_msg("Nastala chyba pri prochzeni souborem!\n");
		return NULL;
	}
	
	/* Soubor je nasledne uzavren s testem uspesnosti a je vracena dana struktura */
	if(fclose(file_ppm) == EOF)
	{
		warning_msg("Soubor se nepodarilo uzavrit\n");
	} 
	return f_ppm;
}


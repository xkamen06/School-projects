/*
 * Predmet: IZP (Zaklady programovani)
 * Projekt: Jendoducha shlukova analyza (clusters)
 * Autor: Zdenek Kamensky 1BIT FIT VUT Brno
 * Datum: 1.12.2015
 */

/**
* Kostra programu pro 3. projekt IZP 2015/16
*
* Jednoducha shlukova analyza: 2D nejblizsi soused.
* Single linkage
* http://is.muni.cz/th/172767/fi_b/5739129/web/web/slsrov.html
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

/*****************************************************************
* Ladici makra. Vypnout jejich efekt lze definici makra
* NDEBUG, napr.:
*   a) pri prekladu argumentem prekladaci -DNDEBUG
*   b) v souboru (na radek pred #include <assert.h>
*      #define NDEBUG
*/
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
* Deklarace potrebnych datovych typu:
*
* TYTO DEKLARACE NEMENTE
*
*   struct objt_t - struktura objektu: identifikator a souradnice
*   struct cluster_t - shluk objektu:
*      pocet objektu ve shluku,
*      kapacita shluku (pocet objektu, pro ktere je rezervovano
*          misto v poli),
*      ukazatel na pole shluku.
*/

struct obj_t {
	int id;
	float x;
	float y;
};

struct cluster_t {
	int size;
	int capacity;
	struct obj_t *obj;
};

/*****************************************************************
* Deklarace potrebnych funkci.
*
* PROTOTYPY FUNKCI NEMENTE
*
* IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
*
*/

/*
Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
	assert(c != NULL);
	assert(cap >= 0);
	c->obj = malloc(cap * sizeof(struct obj_t));
	c->capacity = cap;
	c->size = 0;
}

/*
Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
*/
void clear_cluster(struct cluster_t *c)
{
	c->capacity = 0;
	c->size = 0;
	free(c->obj);
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
*/
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
	// TUTO FUNKCI NEMENTE
	assert(c);
	assert(c->capacity >= 0);
	assert(new_cap >= 0);

	if (c->capacity >= new_cap)
		return c;

	size_t size = sizeof(struct obj_t) * new_cap;

	void *arr = realloc(c->obj, size);
	if (arr == NULL)
		return NULL;

	c->obj = arr;
	c->capacity = new_cap;
	return c;
}

/*
Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
nevejde.
*/
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
	if (c->size == c->capacity)
	{
		resize_cluster(c, (c->capacity) + 1);
	}
	c->obj[c->size] = obj;
	c->size++;
}

/*
Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
*/
void sort_cluster(struct cluster_t *c);

/*
Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
Objekty ve shluku 'c1' budou serazny vzestupne podle identifikacniho cisla.
Shluk 'c2' bude nezmenen.
*/
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
	assert(c1 != NULL);
	assert(c2 != NULL);
	int i = 0;
	while (c2->size > 0)
	{
		append_cluster(c1, c2->obj[i]);
		c2->size = c2->size - 1;
		i++;
	}
	sort_cluster(c1);
	// TODO aby to nezmizelo z puvodniho - zeptat se
}

/**********************************************************************/
/* Prace s polem shluku */

/*
Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
(shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
	assert(idx < narr);
	assert(narr > 0);
	clear_cluster(&carr[idx]);
	while (idx < (narr - 1))
	{
		carr[idx] = carr[idx + 1];
		idx++;
	}
	return narr - 1;
}

/*
Pocita Euklidovskou vzdalenost mezi dvema objekty.
*/
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
	assert(o1 != NULL);
	assert(o2 != NULL);
	float a;
	float b;
	float distance;
	a = (o1->x) - (o2->x);
	a = powf(a, 2.0);
	b = (o1->y) - (o2->y);
	b = powf(b, 2.0);
	distance = sqrtf(a + b);
	return distance;
}

/*
Pocita vzdalenost dvou shluku. Vzdalenost je vypoctena na zaklade nejblizsiho
souseda.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
	assert(c1 != NULL);
	assert(c1->size > 0);
	assert(c2 != NULL);
	assert(c2->size > 0);
	float distance = INFINITY;
	float cou_dis;
	for (int a = 0; a < c1->size; a++)
	{
		for (int b = 0; b < c2->size; b++)
		{
			cou_dis = (obj_distance(&c1->obj[a], &c2->obj[b]));
			if (cou_dis < distance)
			{
				distance = cou_dis;
			}
		}
	}
	return distance;
}

/*
Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
hleda dva nejblizsi shluky (podle nejblizsiho souseda). Nalezene shluky
identifikuje jejich indexy v poli 'carr'. Funkce nalezene shluky (indexy do
pole 'carr') uklada do pameti na adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
	assert(narr > 0);
	float distance = INFINITY;
	float cou_dis;
	int k;
	int l;
	int change_dis = 0;
	for (int a = 0; a < narr; a++)
	{
		for (int b = 1 + a; b < narr; b++)
		{
			cou_dis = (cluster_distance(&carr[a], &carr[b]));
			if (cou_dis < distance)
			{
				change_dis++;
				distance = cou_dis;
				k = a;
				l = b;
			}
		}
	}
	c1 = &k;
	c2 = &l;
	merge_clusters(&carr[*c1], &carr[*c2]);
	remove_cluster(carr, narr, *c2);
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
	// TUTO FUNKCI NEMENTE
	const struct obj_t *o1 = a;
	const struct obj_t *o2 = b;
	if (o1->id < o2->id) return -1;
	if (o1->id > o2->id) return 1;
	return 0;
}

/*
Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
	// TUTO FUNKCI NEMENTE
	qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
	// TUTO FUNKCI NEMENTE
	for (int i = 0; i < c->size; i++)
	{
		if (i) putchar(' ');
		printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
	}
	putchar('\n');
}

/*
Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
	assert(arr != NULL);
	char *strtol_control;
	FILE *f;
	char pole[200];
	char pom_pole[200];
	int count;
	int err_check;
	float check_id; //promenna pro kontolu zda ID neni vetsi nez INT_MAX
	float check_x; //promenna pro kontolu zda souradnice x je v danem intervalu
	float check_y; //promenna pro kontolu zda souradnice y je v danem intervalu
	/* Pokud se nepodari otevrit soubor, vypise chybu */
	if ((f = fopen(filename, "r")) == NULL)
	{
		fprintf(stderr, "Error: Invalid file\n");
		return -1;
	}
	/* Pokud je soubor prazdny, vypise chybui */
	if ((fscanf(f, "%s", pole)) == EOF)
	{
		fprintf(stderr, "Error: Invalid contents of file\n");
		return -1;
	}
	/* Nacteni znaku za 'count=' do pom_pole */
	int i = 0;
	while (pole[i + 6] != '\0')
	{
		pom_pole[i] = pole[i + 6];
		i++;
	}
	while(i < 200)
	{
		pom_pole[i] = '\0';
		i++;
	}
	/* Prevedeni na integer a overeni spravnosti */
	count = strtol(pom_pole, &strtol_control, 10);
	if(*strtol_control != 0)
	{
		fprintf(stderr, "Error: Invalid number of objects\n");
		return -1;
	}
	/* Alokace mista pro vsechny clustery, respektive objekty a overeni spravnosti */
	*arr = malloc(count*sizeof(struct cluster_t));
	if(*arr == NULL)
	{
		fprintf(stderr, "Error: Allocation of field of clusters has failed\n");
		return -1; 
	}
	struct obj_t obj;
	/* Nacteni objektu a prirazeni do clusteru */
	for (int j = 0; j < count; j++)
	{
		/* Pokud je count vetsi nez realny pocet objektu v souboru,
 		 * vypise se chyba  
 		*/
		err_check = (fscanf(f, "%f", &check_id));
		if(err_check == 0 || err_check == EOF || check_id > INT_MAX || check_id != (int)check_id)
		{	
			fprintf(stderr, "Error: Invalid contents of file\n");
			return -1;
		}	
		err_check = (fscanf(f, "%f", &check_x));
		if(err_check == 0 || err_check == EOF || check_x < 0 || check_x > 1000 || check_x != (int)check_x)
		{
			fprintf(stderr, "Error: Invalid contents of file\n");
			return -1;
		}
		err_check = (fscanf(f, "%f", &check_y));
		if(err_check == 0 || err_check == EOF || check_y < 0 || check_y > 1000  || check_y != (int)check_y)
		{
			fprintf(stderr, "Error: Invalid contents of file\n");
			return -1;
		}
		obj.id = (int)check_id;
		obj.x = check_x;
		obj.y = check_y;
		init_cluster(&(*arr)[j], 1);
		append_cluster(&(*arr)[j], obj);
	}
	/* Uzavreni souboru a overeni zda akce probehla v poradku */
	if (fclose(f) == EOF)
	{
		fprintf(stderr, "Error: Can't close the file\n");	
		return -count;
	}
	return count;
}

/*
Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
	printf("Clusters:\n");
	for (int i = 0; i < narr; i++)
	{
		printf("cluster %d: ", i);
		print_cluster(&carr[i]);
	}
}

int main(int argc, char *argv[])
{
	/* Pokud je pocet argumntu vyssi nez 3, vypise se chyba */
	if(argc > 3)
	{
		fprintf(stderr, "Error: Too many arguments\n");
		return -1;
	}
	int a = 0; //pomocna promenna pro while cykly
	char *control;
	struct cluster_t *clusters;
	int c_clust = (load_clusters(argv[1], &clusters));
	/* Pokud hodnota v c_clust je zaporna (funkce load_clusters 
	 * vraci zapornou hodnotu) znaci to chybu a program skonci s -1
	 */
	if(c_clust < 0)
	{	
		return -1;
	}
	/* Pokud je zadan soubor, ale neni zadan pocet pozadovanych clusteru, bere se tento pocet jako 1 */
	int coun_read;
	if(argc < 3)
	{
		coun_read = 1;
	}
	else
	{
		coun_read = strtol(argv[2], &control, 10);
		if(*control != 0)
		{
			while(a < c_clust)
			{
				clear_cluster(&clusters[a]);
				a++;
			}
			free(clusters);
			fprintf(stderr, "Error: Invalid argument\n");
			return -1;
		}
	}
	/* Pokud je pozadovano vypsani vyce clusteru, nez je nactenych objektu, 
 	 * nebo je tento pocet zaporny, vypise se chyba
 	*/
	if(coun_read > c_clust || coun_read <=0)
	{
		while (a < c_clust)
		{
			clear_cluster(&clusters[a]);
			a++;
		}
		free(clusters);
		fprintf(stderr, "Error: Invalid argument\n");
		return -1;
	}
	/* Seskupeni clusteru do pozadovaneho poctu a vytisteni clusteru */ 
	while(c_clust > coun_read)
	{
		find_neighbours(&(*clusters), c_clust, 0, 0);
		c_clust--;
	}
	print_clusters(&(*clusters), coun_read);
	/* Vycisteni */
	while(a < coun_read)
	{
		clear_cluster(&clusters[a]);
		a++;
	}
	free(clusters);
	return 0;
}
/*
 * Predmet: IZP (Zaklady programovani)
 * Projekt: Iteracni vypocty - zretezeny zlomek a tayloruv polynom
 *		+ vypocet poctu iteraci pro vysledek s danou presnosti
 * Autor: Zdenek Kamensky 1BIT FIT VUT Brno
 * Datum: 15.11.2015
 **/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>

/* funkce pro absolutni hodnotu cisla */
double absolut(double hodnota)
{
    /* pokud je vstup mensi jak 0, pricte se (-- = +) jeho dvojnasobek */
    if(hodnota < 0)
    {
        hodnota -= 2 * hodnota;
        return hodnota;
    }
    else
    {
        return hodnota;
    }
}

/* funkce pro vypocet hodnoty taylorova polynomu v danem poctu iteraci */
double taylor_log(double x, unsigned int n)
{
    /* pokud je x INFINITY, funkce vrati na vystup take INFINITY */
    if(isinf(x))
    {
        return INFINITY;
    }
    /* pokud je x 0.0, funkce vraci na vystup -INFINITY */
    if(x == 0.0)
    {
        return -INFINITY;
    }
    /* pokud je x zaporne, funkce vraci na vystup NAN */
    if(x < 0.0)
    {
        return NAN;
    }

    /* promenne pro soucet clenu a citatel zlomku */
    double soucet = 0;
    double cit = 1;
    /* pokud je zadana hodnota v intervalu (0,1>, provadi se nasledujici struktura */
    if(x <= 1)
    {
        double moc;
        x = 1-x; /* uprava hodnoty x do podoby, se kterou se bude pocitat */
        moc = x; /* nastaveni hodnoty, kterou se bude mocnit */
        for (unsigned int a = 1; a <= n; a++)
        {
            cit = cit * moc;
            x = - (cit/a);
            soucet = soucet + x;
        }
        return soucet;
    }
    /* pokud je zadana hodnota naopak vetsi jak 1,
       provadi se nasledujici struktura */
    else
    {
        double zlomek;
        x = (x - 1) / x; /* upravena hodnota x */
        for (unsigned int a = 1; a <= n; a++ )
        {
            cit = cit * x;
            zlomek = cit / a;
            soucet = soucet + zlomek;
        }
        return soucet;
    }
}

/* funkce pro vypocet zretezeneho zlomku v danem poctu iteraci */
double cfrac_log(double x, unsigned int n)
{
    /* pro nulty pocet iteraci se vypise chybova hlaska */
    if(n == 0)
    {
        return 1;
    }
    /* pokud je x INFINITY, funkce vrati na vystup take INFINITY */
    if(isinf(x))
    {
        return INFINITY;
    }
    /* pokud je x 0.0, funkce vraci na vystup -INFINITY */
    if(x == 0.0)
    {
        return -INFINITY;
    }
    /* pokud je x zaporne, funkce vraci na vystup NAN */
    if(x < 0)
    {
        return NAN;
    }

    double K; /* hodnota, od ktere se ve jmenovateli odetacita predchozi clen */
    double A; /* soucasny clen po iterovani */
    double jmen; /* jmenovatel */
    double cit; /* citatel */
    x = (x - 1)/(x + 1); /* upraveni x do pozadovane podoby */
    for (unsigned int i = n + 1; i >= 2; i--)
    {
        K = (2 * i) - 1;
        jmen = K - A;
        cit = x*x*(i - 1)*(i - 1);
        A = cit/jmen;
    }

    /* zaverecna iterace */
    jmen = 1 - A;
    cit = 2 * x;
    A = cit / jmen;

    return A;
}

/* funkce pro zjisteni poctu iteraci pro dosazeni pozadovane
   presnosti (absolutni hodnota rozdilu aktualni hodnoty po dane
   iteraci a hodnoty logaritmu daneho cisla) u zretezeneho zlomku */
double cf_iter(double x, double EPS)
{
    double vysl = 0;
    unsigned int poc_iter = 0;
    do
    {
        /* opakovane vola funkci cfrac_log pro ruzny (posloupny)
           pocet iteraci */
        vysl = (cfrac_log(x, (poc_iter + 1)));
        poc_iter++;
    } while(absolut(vysl - log(x)) >= EPS);
    return poc_iter; /* vraci pocet nutnych iteraci */
}

/* funkce pro zjisteni poctu iteraci pro dosazeni pozadovane
   presnosti u taylorova polynomu */
double taylor_iter(double x, double EPS)
{
    double vysl = 0;
    unsigned int poc_iter = 0;
    do
    {
        vysl = (taylor_log(x, (poc_iter + 1)));
        poc_iter++;
    } while(absolut(vysl - log(x)) >= EPS);
    return  poc_iter;/* funkce vraci pocet iteraci */
}

int main(int argc, char *argv[])
{
    char *kontrol;
    errno = 0;

    /* pokud je pocet argumentu celkem 4 a 1. zadany argument je "--log",
       provede se nasledujici struktura */
    if(argc == 4 && strcmp(argv[1], "--log") == 0)
    {
        /* promenna x predsavuje realne cislo, jehoz logaritmus bude pocitan
           a promenna n je dany pocet iteraci (opakovani), ktery bude provaden */
        double x;
        unsigned int n;

        /* do promenne test je vlozen pocet iteraci - pokud je zaporny, program
           vypise chybovou hlasku */
        double test;
        test = strtod(argv[3], &kontrol);
        if(test < 0)
        {
            printf("Chyba: Zadan zaporny pocet iteraci!\n");
            return 1;
        }
        /* kontrola zda hodnota v argv[2] je je hodnotou typu double(bez pismen a jinych znaku)
           a nacteni hodnoty do promenne x */
        x = strtod(argv[2], &kontrol);
        if(*kontrol != 0 || errno == ERANGE)
        {
            printf("Chyba: Zadany neplatne argumenty - realne cislo!\n");
            return 1;
        }
        /* kontrola zda hodnota v argv[3] je je hodnotou typu unsigned int v desitkove soustave (bez pismen
           a jinych znaku) a nacteni promenne do hodnoty n */
        n = strtoul(argv[3], &kontrol, 10);
        if(*kontrol != 0 || errno == ERANGE)
        {
            printf("Chyba: Zadany neplatne argumenty - pocet iteraci!\n");
            return 1;
        }
        /* pokud funkce cfrac_log vraci hodnotu 1, je zadan nulovy pocet iteraci
           a program vypise chybovou hlasku */
        double cf_l = (cfrac_log(x, n));
        if(cf_l == 1)
        {
            printf("Chyba: Byl zadan nulovy pocet iteraci!\n");
        }
        else
        {
            /* vola se funkce z knihovny <math.h>, ktera vypocita prirozeny logaritmus
               ze zadane hodnoty */
            printf("log(%g) = %.12g\n", x, log(x));
            /* vola se funkce pro vypocet hodnoty taylorova polynomu a zretzeneho zlomku */
            printf("cf_log(%g) = %.12g\n", x, cf_l);
            printf("taylor_log(%g) = %.12g\n", x, (taylor_log(x, n)));
        }
    }

    /* pokud je pocet argumentu roven 5 a 1. zadany argument je "--iter",
       pak se provede nasledujici struktura */
    else if(argc == 5 && strcmp(argv[1], "--iter") == 0)
    {
        /* minimalni (MIN) a maxmalni (MAX) hodnoty intervalu a pozadovane odchylky (EPS) */
        double MIN;
        double MAX;
        double EPS;
        /* pocet iteraci pro pozadovanou ochylku u MIN a MAX */
        unsigned int iter_MIN;
        unsigned int iter_MAX;

        /* kontrola zda hodnota v argv[2] je je hodnotou typu double(bez pismen a jinych znaku)
           a zda je tato hodnota kladna nenulova */
        MIN = strtod(argv[2], &kontrol);
        if(*kontrol != 0 || errno == ERANGE)
        {
            printf("Chyba: Zadany neplatne argumenty - realne cislo!\n");
            return 1;
        }
        if(MIN <= 0)
        {
            printf("Chyba: Neexistuje logaritmus zaporne, nebo nulove hodnoty!\n");
            return 1;
        }

        /* kontrola zda hodnota v argv[3] je je hodnotou typu double(bez pismen a jinych znaku)
           a zda je tato hodnota kladna nenulova */
        MAX = strtod(argv[3], &kontrol);
        if(*kontrol != 0 || errno == ERANGE)
        {
            printf("Chyba: Zadany neplatne argumenty - realne cislo!\n");
            return 1;
        }
        if(MAX <= 0)
        {
            printf("Chyba: Neexistuje logaritmus zaporne, nebo nulove hodnoty!\n");
            return 1;
        }

        /* kontrola zda hodnota v argv[4] je je hodnotou typu double(bez pismen a jinych znaku)
           a zda je tato hodnota kladna nenulova */
        EPS = strtod(argv[4], &kontrol);
        if(*kontrol != 0 || errno == ERANGE)
        {
            printf("Chyba: Zadany neplatne argumenty - EPS!\n");
            return 1;
        }
        if(EPS <= 0)
        {
            printf("Chyba: Zadana nulova nabo zaporna hodnota EPS!\n");
            return 1;
        }

        if(EPS < 1e-13)
        {
            printf("Chyba: Nebyly zadany platne argumenty!\n"
                   "Pozn.: Zadana odchylka je mensi nez 1e-12.\n");
            return 1;
        }

        /* tisk hodnoty logaritmu z knihovny <math.h> pro obe zadane hodnoty */
        printf("log(%g) = %.12g\n", MIN, log(MIN));
        printf("log(%g) = %.12g\n", MAX, log(MAX));

        /* zjisteni poctu iteraci pomoci volani funkce cf_iter */
        iter_MIN = (cf_iter(MIN, EPS));
        iter_MAX = (cf_iter(MAX, EPS));
        /* vyssi pocet iteraci se vystiskne - vyssi proto, aby byla jistota,
           ze pro kazdou hodnotu z daneho intervalu dojdeme k pozadovane odchylce */
        if(iter_MIN < iter_MAX)
        {
            printf("continued fractions iterations = %u\n", iter_MAX);
        }
        else
        {
            printf("continued fractions iterations = %u\n", iter_MIN);
        }
        /* vytisknou se logaritmy pro dany pocet iteraci */
        printf("cf_log(%g) = %.12g\n", MIN, (cfrac_log(MIN, iter_MIN)));
        printf("cf_log(%g) = %.12g\n", MAX, (cfrac_log(MAX, iter_MAX)));

        /* zjisteni poctu iteraci pomoci volani funkce taylor_iter */
        iter_MIN = (taylor_iter(MIN, EPS));
        iter_MAX = (taylor_iter(MAX, EPS));
        /* vyssi pocet iteraci se vystiskne - vyssi proto, aby byla jistota,
           ze pro kazdou hodnotu z daneho intervalu dojdeme k pozadovane odchylce */
        if(iter_MIN < iter_MAX)
        {
            printf("taylor polynomial iterations = %u\n", iter_MAX);
        }
        else
        {
            printf("taylor polynomial iterations = %u\n", iter_MIN);
        }
        /* vytisknou se logaritmy pro dany pocet iteraci */
        printf("taylor_log(%g) = %.12g\n", MIN, (taylor_log(MIN, iter_MIN)));
        printf("taylor_log(%g) = %.12g\n", MAX, (taylor_log(MAX, iter_MAX)));
    }

    /* pokud je program spusten s neplatnymy argumenty vypise se chyba */
    else
    {
        printf("Chyba: Nebyly zadany platne argumenty!\n");
        return 1;
    }

    return 0;
}
/**
 * Predmet: IZP (Zaklady programovani)
 * Projekt: Prace s textem - Analyza vstupniho textu (slovo, palindrom, cislo, prvocislo, datum, den v tydnu)
 * Autor: Zdenek Kamensky 1BIT FIT VUT Brno
 * Datum: 25.10.2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct tm time_str;

char den[20];

/* funkce pro detekci cisla */
int detekceCisla (char znak)
{
    int k = znak;
    /* postupne porovnani decimalni hodnoty znaku s decimalni hodnotou vsech cisel v tabulce ASCII */
    for (int j = 48; j < 58; j++)
    {
            if(k == j)
            {
                return 0;
            }
    }
    return 1;
}

/* funkce pro detekci prvocisla */
int detekcePrvocisla (int cislo)
{
    /* postupne otestujeme celociselnou delitelnost beze zbytku (%) cisla
       vsemi mensimi celymi cisly (vyjma 1 - inicializujeme a na cislo 2) */
    for (int a = 2; a < cislo; a++)
    {
        if (cislo % a == 0)
        {
            return 1;
        }
    }
    return 0;
}

/* funkce pro detekci palindromu */
int detekcePalindromu (char *slovo, int pocetZnaku)
{
    int palindrom = 0;
    /* testujeme symetrickou shodu znaku v prvni polovine slova se znaky v druhe polovine */
    for (int a = 0; a < (pocetZnaku / 2); a++)
    {
        /* posledni znak slova do testovani nezahrnujeme, jelikoz je to '/0' */
        if (slovo[a] == slovo[pocetZnaku - (a + 1)])
        {
            palindrom++;
        }
    }
    /* pokud pocet nalezenych shod se rovna polovine delky slova pak se jedna o palindrom */
    if (palindrom == (pocetZnaku / 2))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/* funkce pro zjisteni, jestli dane cislo je mensi nebo rovno maximalni hodnote integeru */
int testIntu (char *slovo, char *testInt, int pocetNul)
{
    int i = pocetNul;
    int a = 0;
    while ((slovo[i]) != '\0')
    {
        if (slovo[i] != testInt[a])
        {
            return 1;
        }
        i++;
        a++;
    }
    return 0;
}

/* funkce pro detekci datumu */
char *detekceDatumu (char *slovo)
{
        char rokRetezec [5] = {slovo[0],slovo[1],slovo[2], slovo[3]};
        int rokCislo = atoi(rokRetezec);
        char mesicRetezec [3] = {slovo[5], slovo [6]};
        int mesicCislo = atoi(mesicRetezec);
        char denRetezec [3] = {slovo [8], slovo [9]};
        int denCislo = atoi(denRetezec);
        time_str.tm_year = rokCislo - 1900;
        time_str.tm_mon = mesicCislo-1;
        time_str.tm_mday = denCislo;
        time_str.tm_hour = 0;
        time_str.tm_min = 0;
        time_str.tm_sec = 1;
        time_str.tm_isdst = -1;
        if (denCislo <= 31 && mesicCislo <= 12 && rokCislo >= 1900)
        {
            if (mktime(&time_str) == -1)
            {
                return slovo;
            }
            else
            {
                (void) strftime(den, sizeof(den), "%A", &time_str);
                return den;
            }
        }
        else
        {
            return slovo;
        }
}
/* main */
int main(int argc, char *argv[])
{
    (void)argv;
    /* pokud program bude spusten s argumenty, vypise se napoveda */
    if (argc>1)
    {
        printf("autor: Zdenek Kamensky\n"
               "Tento program byl vytvoren v ramci 1. projektu v predmetu Zaklady programovani\n"
               "v 1. rocniku bakalarskeho studia na Fakulte informacnich technologii na VUT v Brne.\n"
               "\n"
               "Program zpracuje text a urci, zda se v nem nachazeji cisla, slova nebo datum.\n"
               "U kazdeho cisla se provede jeho detekce na prvocislo a u datumu se vypise prislusny den v tydnu.\n");
    }
    else
    {
        /* nacteni slova do pole o maximalni delce 101 znaku
           (musime brat v potaz znak konce slova \0) */
        char slovo[101];
        int i = 0;
        int pocetZnaku = 0;
        int pocetCisel = 0;
        int nacteni = scanf("%100s", slovo);
        while (nacteni == 1)
        {
            /* zjisteni, zda uz neni konec slova - '\0' */
            while (slovo[i] != '\0')
            {
                /* pokud aktualni znak neni \0, zvysi se promenna s realnym poctem
                   znaku (bez '\0') ve slove (timto se zjisti kolik ma slovo skutecne znaku) */
                pocetZnaku++;
                /* volani funkce, ktera overi, zda znak je cele cislo od 0 do 9 */
                if ((detekceCisla(slovo[i])) == 0)
                {
                    pocetCisel++;
                }
                /* posunuti na dalsi znak ve slove */
                i++;
            }
            i = 0;
            int pocetNul = 0;
            for (int a = 0; a < pocetCisel; a++)
            {
                if (slovo[a] == '0')
                {
                    pocetNul++;
                }
                else
                {
                    a = pocetCisel;
                }
            }

            /* funkce atoi prevede promennou slovo (pole) do promenne cislo (integer) */
            int cislo = atoi(slovo);
            
            char testInt [101];
            /*prevedeni z integeru (cislo) opet do pole (testInt) - pomoci funkce sprintf */
            sprintf(testInt, "%d", cislo);
            
            /* pokud je hodnota v promenne slovo cislo a jestlize toto cislo je mensi nebo rovno nez
               maximalni hodnota integeru, pak se provadi test, zda je to prvocislo */
            if (pocetCisel == pocetZnaku && (testIntu(slovo, testInt, pocetNul)) == 0)
            {
                if ((detekcePrvocisla(cislo)) == 0)
                {
                    printf("number: %d (prime)\n", cislo);
                }
                else
                {
                    printf("number: %d\n", cislo);
                }
            }
            /* pokud je hodnota v promenne slovo cislo ale zaroven toto cislo je vetsi nez
               maximalni hodnota integeru, pak se test na prvocislo neprovadi a vypise se pouze, ze je to cislo */
            else if (pocetCisel == pocetZnaku && (testIntu(slovo, testInt, pocetNul)) == 1)
            {
                int b;
                int a;
                char uprSlovo [101];
                for (a = 0; a< (pocetCisel-pocetNul); a++)
                {
                    b = pocetNul + a;
                    uprSlovo [a] = slovo[b];
                }
                uprSlovo [a] = '\0';


                printf("number: %s\n", uprSlovo );/*uprSlovo*/
            }
            /* datum zadavame ve tvaru DDDD-DD-DD, program zjisti, zda na pozici 4 a 7 je znak '-'
               a zda je v tomto slove 10 znaku a z toho 8 cisel, pokud ano vola se funkce detekceDatumu */
            else if (pocetZnaku == 10 && pocetCisel == 8 && slovo [4] == '-' && slovo [7] == '-')
            {
                if ((detekceDatumu(slovo)) != slovo)
                {
                    char *datum  = (detekceDatumu(slovo));
                    char zkrtDatum [4] = {datum[0],datum[1], datum[2]};
                    printf("date: %s %s\n", zkrtDatum, slovo);
                }
                else
                {
                    printf("word: %s\n", slovo);
                }
            }
            /* pokud to nni cislo ani datum, pak je to slovo
               a provadime detekci palindromu volanim funkce detekcePalindromu */
            else
            {
                if ((detekcePalindromu(slovo, pocetZnaku)) == 0)
                {
                    printf("word: %s (palindrome)\n", slovo);
                }
                else
                {
                    printf("word: %s\n", slovo);
                }
            }
            /* vynuluji se promenne */
            pocetCisel = 0;
            pocetZnaku = 0;
            /*nacita se dalsi slovo a vse se znovu opakuje */
            nacteni = scanf("%100s", slovo);
        }
    }
    return 0;
}
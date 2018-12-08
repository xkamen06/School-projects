/*
 *Soubor: tst_math_utests.cpp
 *Tym: Vorezpruti
 *Autor: Zdenek Kamensky, 1.rocnik, FIT VUT Brno
 *Datum: 15.4.2016
 *Ukol: 2. projekt do predmetu Prakticke aspekty vyvoje software (IVS)
 *      skupinovy projekt - kalkulacka, vcetne matematicke knihovny a jejich testu,
 *	zaznamu o debuggovani a profilovani, softwarova a uzivatelska dokumentace, ...
 *Popis souboru: Testy matematicke knihovny "mathematic_lib.h"
 *
 */ 


#include <QtTest>
#include "mathematic_lib.h" //testovana knihovna


/********** TRIDA PRO TESTOVACI FUNKCE MATEMATICKE KNIHOVNY **********/
class math_utests : public QObject
{
    Q_OBJECT

//prototypy testovacich funkci
private Q_SLOTS:
    void test_add_f();
    void test_sub_f();
    void test_mul_f();
    void test_div_f();
    void test_fact_f();
    void test_pow_f();
    void test_sqrt_f();
}; 


/***** TESTY FUNKCE ADD_F + FUNKCE PRO SOUCET DVOU CISEL *****/
void math_utests::test_add_f()
{

 /*Obycejna dve cela cisla: */
    QCOMPARE((double)10, add_f((double)2, (double)8)); 

 /*Dve velka cisla: */   
    QCOMPARE((double)6050000, add_f((double)(50000), (double)6000000)); 

 /* Dve nulova cisla: */
    QCOMPARE((double)0, add_f((double)0, (double)0)); 
    
 /*Jedno nulove cislo: */
    QCOMPARE((double)42, add_f((double)42, (double)0)); 
    
 /*Jedno zaporne a jedno kladne cislo: */
    QCOMPARE((double)16, add_f((double)(-4), (double)20));

 /*Dve zaporna cisla: */
    QCOMPARE((double)(-4), add_f((double)(-2), (double)(-2))); 

 /*Jedno realne a jedno cele cislo: */
    QCOMPARE((double)13.32, add_f((double)2.32, (double)11));
 
 /*Dve realna cisla: */
    QCOMPARE((double)453.0076, add_f((double)253.007, (double)200.0006)); 

}


/***** TESTY FUNKCE SUB_F + FUNKCE PRO ROZDIL DVOU CISEL *****/
void math_utests::test_sub_f()
{
 
 /*Odecteni mensiho cisla od vetsiho: */
    QCOMPARE((double)5, sub_f((double)10, (double)5));

 /*Odecteni vetsiho cisla od mensiho: */
    QCOMPARE((double)(-5), sub_f((double)5, (double)10));

 /*Rozdil dvou velkych cisel: */
    QCOMPARE((double)(-10326), sub_f((double)1050324, (double)1060650));

 /*Odecteni zaporneho cisla od kladneho (soucet): */
   QCOMPARE((double)12, sub_f((double)5, (double)(-7)));

 /*Odecteni kladneho cisla od zaporneho: */ 
   QCOMPARE((double)(-12), sub_f((double)(-7), (double)5));

 /*Rozdil dvou zapornych cisel: */
   QCOMPARE((double)(-7), sub_f((double)(-10), (double)(-3)));

 /*Odecteni obycejneho cisla od nuloveho cisla: */
   QCOMPARE((double)(-3), sub_f((double)0, (double)(3)));

 /*Rozdil realneho a celeho cisla: */
   QCOMPARE((double)5.7, sub_f((double)12, (double)6.3));

 /*Rozdil dvou realnych cisel: */
   QCOMPARE((double)(-17.5487), sub_f((double)3.08986, (double)20.63856));

 /*Rozdil dvou nulovych cisel: */
   QCOMPARE((double)0, sub_f((double)0, (double)0)); 

}


/***** TESTY FUNKCE MUL_F + FUNKCE PRO NASOBENI DVOU CISEL *****/
void math_utests::test_mul_f()
{

 /*Nasobeni dvou obycejnych cisel: */
    QCOMPARE((double)39, mul_f((double)13, (double)3));

 /*Nasobeni dvou velkych cisel: */
    QCOMPARE((double)(130130000), mul_f((double)1001, (double)130000));

 /*Nasobeni jednoho zaporneho a jednoho kladneho cisla: */
   QCOMPARE((double)(-35), mul_f((double)5, (double)(-7)));

 /*Nasobeni dvou zapornych cisel: */
   QCOMPARE((double)(30), mul_f((double)(-10), (double)(-3)));

 /*Nasobeni obycejneho cisla a nuloveho cisla: */
   QCOMPARE((double)(0), mul_f((double)0, (double)(300)));

 /*Nasobeni realneho a celeho cisla: */
   QCOMPARE((double)12.6, mul_f((double)2, (double)6.3));

 /*Nasobeni dvou realnych cisel: */
   QCOMPARE((double)(4.502616), mul_f((double)13.089, (double)0.344));

 /*Nasobeni dvou nulovych cisel: */
   QCOMPARE((double)0, mul_f((double)0, (double)0)); 
 
}


/***** TESTY FUNKCE DIV_F + FUNKCE PRO DELENI DVOU CISEL *****/
void math_utests::test_div_f()
{

 /*Deleni dvou obycejnych cisel: */
    QCOMPARE((double)10, div_f((double)100, (double)10));

 /*Deleni mensiho cisla vetsim: */
    QCOMPARE((double)0.01, div_f((double)10, (double)1000));

 /*Deleni dvou velkych cisel: */
    QCOMPARE((double)61000, div_f((double)36600000, (double)600));

 /*Deleni zaporneho cisla kladnym cislem: */
   QCOMPARE((double)(-6), div_f((double)(-42), (double)7));

 /*Deleni zaporneho cisla zapornym cislem: */
   QCOMPARE((double)35, div_f((double)(-3395), (double)(-97)));

 /*Deleni nuloveho cisla celym cislem: */
   QCOMPARE((double)0, div_f((double)0, (double)8332));

 /*Deleni realneho cisla celym cislem: */
   QCOMPARE((double)(21.091), div_f((double)(42.182), (double)2));

 /*Deleni celeho cisla realnym cislem: */
   QCOMPARE((double)(10), div_f((double)(42), (double)4.2));

 /*Deleni dvou realnych cisel: */
   QCOMPARE((double)(2), div_f((double)(58.6), (double)29.3));

}


/***** TESTY FUNKCE FACT_F + FUNKCE PRO VYPOCET FAKTORIALU *****/
void math_utests::test_fact_f()
{
 
 /*Faktorial obycejneho cisla: */
    QCOMPARE((double)24, fact_f((double)4));
 
 /*Faktorial zaporneho cisla: */
    QCOMPARE((double)(-2), fact_f((double)(-12))); //OSETRENI CHYBNEHO VSTUPU

 /*Faktorial nuly: */
    QCOMPARE((double)1, fact_f((double)0));

 /*Faktorial desetinneho cisla: */
    QCOMPARE((double)(-1), fact_f((double)43.54)); //OSETRENI CHYBNEHO VSTUPU

}


/***** TESTY FUNKCE POW_F + FUNKCE PRO VYPOCET N-TE MOCNINY ZADANEHO CISLA *****/
void math_utests::test_pow_f()
{

 /**POZNAMKA: V KALKULACCE LZE UMOCNIT POUZE NA CELOCISELNE EXPONENTY!!!**/

 /*Cele cislo umocnene na obycejny exponent: */	
    QCOMPARE((double)3375, pow_f((double)15, 3));

 /*Cele cislo umocnene na vysoky exponent: */
    QCOMPARE((double)4294967296, pow_f((double)2, 32));

 /*Cele cislo umocnene na nulovy exponent: */
    QCOMPARE((double)1, pow_f((double)452, 0));

 /*Cele cislo umocnene na prvnou: */
    QCOMPARE((double)13, pow_f((double)13, 1));

 /*Cele cislo umocnene na zaporny exponent: */
    QCOMPARE((double)0.125, pow_f((double)2, (-3)));

 /*Realne cislo umocnene na obycejny exponent: */
    QCOMPARE((double)9.7344, pow_f((double)3.12, 2));

 /*Realne cislo umocnene na zaporny expoenent: */
    QCOMPARE((double)4, pow_f((double)0.5, (-2)));

}


/***** TESTY FUNKCE SQRT_F + FUNKCE PRO VYPOCET ODMOCNINY *****/
void math_utests::test_sqrt_f()
{
 
 /**POZNAMKA: ODMOCNINA LZE VYPOCITAT POUZE Z CELEHO CISLA!!!**/

 /*Odmocnina obycejneho cisla: */
    QCOMPARE((double)4, sqrt_f((double)16));

 /*Odmocnina nuly: */
    QCOMPARE((double)0, sqrt_f((double)0));

 /*Odmocnina ze zaporneho cisla: */
    QCOMPARE((double)(-3), sqrt_f((double)(-25))); //OSETRENI CHYBOVEHO VSTUPU

}


QTEST_APPLESS_MAIN(math_utests) //Provede veskere testy automaticky (improvizovana funkce main)

#include "tst_math_utests.moc"

/*
 * File: pqueuetest.h
 * ------------------
 * Den här modulen exporterar ett antal funktiner som testar
 * olika operatiner på en pqueueADT. Funktionerna är
 * användbara för att testa om en ny implementering är korrekt.
 *
 * Niclas Åstrand, Data- och programstrukturer HT-2003
 * Senast modifierad: 23/7 - 2003
 */

#ifndef _pqueuetest_h
#define _pqueuetest_h

#include "pqueue.h"

/*
 * Function: BasicPQueueTest
 * Usage: BasicQueueTest();
 * ------------------------
 * Testar enkla Enqueue och DequeueMax.
 * Resultatet skrivs ut till konsolen.
 */

void BasicPQueueTest(void);

/*
 * Function: MoreQueueTest
 * Usage: MoreQueueTest();
 * -----------------------
 * Testar ytterligare Enqueue, DequeueMax, en del 'boundry cases'
 * testas även.
 * Resultatet skrivs ut till konsolen.
 */

void MorePQueueTest();

/*
 * Function: PQueueSortTest
 * Usage: PQueueSortTest();
 * ------------------------
 * Testar användning av prioritetskön genom att implementera sorterings-
 * algoritmen HeapSort. Funktionen sätter in en mängd värden i kön
 * för att sedan använda DequeueMax för att plocka ut elementen
 * och placera dem i ett fält i sorterad ordning. Utför först operationen
 * med slumpade värden. Operationen utförs sedantvå gånger till genom
 * att sätta in värden i sorterad ordning (både fram- och baklänges) för
 * att avgöra hur operationen påverkas av data som redan är sorterad.
 */

void PQueueSortTest();

/*
 * Function: Heapsort
 * Usage: Heapsort(pq, arr, n);
 * ----------------------------
 * Använder prioritetskön för att implementera algoritmen HeapSort.
 * Algoritmen sätter in alla element i array i prioritetskön för
 * att sedan plocka ut dem med DequeueMax och placera tillbaka dem
 * i array i stigande ordning. Beroende på den underliggande
 * representationen av kön så kan detta vara en relativt snabb
 * sorteringsalgoritm. Funktionen exporteras eftersom den används
 * av performance.c såväl som den här modulen.
 */

void HeapSort(pqueueADT pq, int array[], int nElements);

#endif

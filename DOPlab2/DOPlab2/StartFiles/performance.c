/*
 * File: performance.c
 * -------------------
 * Funktionerna i den här modulen är avsedda att testa effektiviteten
 * på olika implementeringar av en pqueueADT. Funktionerna testar
 * tidskomplexitet och minnesförbrukning. Det finns ingen anledning
 * att förändra någon kod i den här modulen.
 *
 * Niclas Åstrand, Data- och programstrukturer HT-2003
 * Senast modifierad: 25/4 - 2010
 */

#include <stdio.h>
#include <time.h>
#include "genlib.h"
#include "random.h"
#include "simpio.h"
#include "pqueue.h"
#include "performance.h"
#include "pqueuetest.h"

/* Parameters for the Enqueue and DequeueMax time measurement */
#define NUM_PQS  20
#define REPS_FRAC 0.1f

/* Parameters for the random Enqueue/DequeueMax performance test */
#define MIN_TRIAL_SIZE      1000
#define MAX_TRIAL_SIZE      1000000
#define NUM_OPERATIONS      1000
#define NUM_REPETITIONS     10       /* Repeat the sort experiments to get better timing accuracy. */


static void RunEnqueueDequeueTrial(int size);
static void RunSortTrial(int size);
static void RunMemoryTrial(int size);
static double GetCurrentCPUTime(void);
static void RunPerformanceTrial(int size);

void PQueuePerformance(void)
{
    int size;

    printf("\nNow, let's run an empirical time trial.\n");

    while (TRUE) {
        while (TRUE) {
            printf("\nHow large a priority queue to time? (%d to %d or -1 to quit): ",
                   MIN_TRIAL_SIZE, MAX_TRIAL_SIZE);
            size = GetInteger();
            if (size == -1) return;
            if (size >= MIN_TRIAL_SIZE && size <= MAX_TRIAL_SIZE) break;
            printf("Please use a size in the supported range.\n");
        }
        RunPerformanceTrial(size);
    }
}

/*
 * Function: RunPerformanceTrial
 * -----------------------------
 * Funktionen provkör prioritetsköer av angivet storlek. De 2 första
 * provkörningarna rapporterar tidsåtgången för operationerna enqueue,
 * dequeue och sortering. Den sista rapporterar hur mycket minne kön
 * har förbrukat.
 */

static void RunPerformanceTrial(int size)
{
    Randomize();
    printf("\n---------- Performance trial for %d-element queue -----------\n\n", size);
    RunEnqueueDequeueTrial(size);
    RunSortTrial(size);
    RunMemoryTrial(size);
    printf("\n------------------- End of trial ---------------------\n\n");
}

/*
 * Function: RunEnqueueDequeueTrial
 * --------------------------------
 * Kör tidtagningar på operationerna enqueue och dequeue för
 * en prioritetskö av en angiven storlek. Resultatet skrivs ut till
 * konsolen. Notera att funktionen vid insättning i kön använder
 * slumptal. Tidsåtgången för att utföra en enqueue/dequeue är
 * för liten för att mäta så funktionen använder flera iterationer
 * för att mäta tidsåtgången.
 *
 * [Anders Gidenstam, 2009]: Notera att kölängden inte bör öka/minska för mycket under experimentet.
 * För att undvika detta delar vi upp operationerna över flera prioritetsköer.
 */

static void RunEnqueueDequeueTrial(int size)
{
    int i, j;
    double start;
    pqueueADT pq[NUM_PQS];
    int NUM_REPS = (int)(REPS_FRAC*size);

    for (i = 0; i < NUM_PQS; i++) {
        pq[i] = NewPQueue();
    }

    for (i = 0; i < NUM_PQS; i++) {
        for (j = 0; j < size; j++) {
            Enqueue(pq[i], RandomInteger(1, size));
        }
    }

    start = GetCurrentCPUTime();
    for (i = 0; i < NUM_PQS; i++) {
        for (j = 0; j < NUM_REPS; j++) {
            Enqueue(pq[i], RandomInteger(1, 2*size));
        }
    }
    printf("Time to enqueue into %d-element priority queue: %g usecs\n", size,
           1000.0*(GetCurrentCPUTime()-start)/(NUM_PQS * NUM_REPS));

    start = GetCurrentCPUTime();
    for (i = 0; i < NUM_PQS; i++) {
        for (j = 0; j < NUM_REPS; j++) {
            DequeueMax(pq[i]);
        }
    }
    printf("Time to dequeue from %d-element priority queue: %g usecs\n", size,
           1000*(GetCurrentCPUTime()-start)/(NUM_PQS * NUM_REPS));

    for (i = 0; i < NUM_PQS; i++) {
        FreePQueue(pq[i]);
    }
}

/*
 * Function: RunSortTrial
 * ----------------------
 * Funktinen kör tidtagningar för sortering på en prioritetskö
 * av angiven storlek. Resultatet skrivs till konsolen.
 */

static void RunSortTrial(int size)
{
    int i, rep;
    int *array[NUM_REPETITIONS];
    double start;
    pqueueADT pq[NUM_REPETITIONS];

    for (rep = 0; rep < NUM_REPETITIONS; rep++) {
        array[rep] = NewArray(size, int);
        for (i = 0; i < size; i++)
            array[rep][i] = RandomInteger(1, size);
        pq[rep] = NewPQueue();
    }

 
    start = GetCurrentCPUTime();
    for (rep = 0; rep < NUM_REPETITIONS; rep++) {
        HeapSort(pq[rep], array[rep], size);
    }
    printf("Time to heapsort random %d-element array using priority queue: %g msecs\n", size,
           (GetCurrentCPUTime() - start)/NUM_REPETITIONS);

    for (rep = 0; rep < NUM_REPETITIONS; rep++) {
        for (i = 0; i < size; i++)
            array[rep][i] = i;
        /* Recreate the priority queues to avoid any reuse bias (e.g. due to automtic resize). */
        FreePQueue(pq[rep]);
        pq[rep] = NewPQueue();
    }

    start = GetCurrentCPUTime();
    for (rep = 0; rep < NUM_REPETITIONS; rep++) {
        HeapSort(pq[rep], array[rep], size);
    }
    printf("Time to heapsort sorted (increasing order) %d-element array using priority queue: %g msecs\n", size,
           (GetCurrentCPUTime() - start)/NUM_REPETITIONS);

    for (rep = 0; rep < NUM_REPETITIONS; rep++) {
        for (i = 0; i < size; i++)
            array[rep][i] = size - i;
        /* Recreate the priority queues to avoid any reuse bias (e.g. due to automtic resize). */
        FreePQueue(pq[rep]);
        pq[rep] = NewPQueue();
    }

    start = GetCurrentCPUTime();
    for (rep = 0; rep < NUM_REPETITIONS; rep++) {
        HeapSort(pq[rep], array[rep], size);
    }
    printf("Time to heapsort reverse-sorted (decreasing order) %d-element array using priority queue: %g msecs\n", size,
           (GetCurrentCPUTime() - start)/NUM_REPETITIONS);

    for (rep = 0; rep < NUM_REPETITIONS; rep++) {
        FreeBlock(array[rep]);
        FreePQueue(pq[rep]);
    }
}

/*
 * Function: RunMemoryTrial
 * ------------------------
 * Funktionen fyller en prioritetskö med angivet antal element (size) och
 * skriver ut minnesförbrukningen till konsolen. Då insättning i prioritetskön
 * av angivet antal element genomförts sker ytterligare en mängd
 * enqueue-/dequeueoperationer och minnesförbrukningen skrivs åter till
 * konsolen.
 */

static void RunMemoryTrial(int size)
{
    int i, num;
    pqueueADT pq;

    pq = NewPQueue();
    for (i = 0; i < size; i++)
        Enqueue(pq, i);

    printf("\nAfter consecutive enqueues, %d-element priority queue is using %d KB of memory\n",
           size, BytesUsed(pq)/1000);

    num = size;
    for (i = 0; i < NUM_OPERATIONS; i++) {
        if (RandomChance(.5)) {
            Enqueue(pq, RandomInteger(0, size));
            num++;
        } else {
            DequeueMax(pq);
            num--;
        }
    }
    printf("After more enqueue/dequeue, %d-element priority queue is using %d KB of memory\n",
           num, BytesUsed(pq)/1000);

    FreePQueue(pq);
}

/*
 * Function: GetCurrentCPUTime()
 * -----------------------------
 * Funktionen returnerar en approximation av CPU-tiden i millisekunder som
 * programmet har använt.
 */

static double GetCurrentCPUTime()
{
    return (clock() * 1000.0/CLOCKS_PER_SEC);
}

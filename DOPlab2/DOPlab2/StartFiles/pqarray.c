/*
 * File: pqueue.c
 * --------------
 * Den här filen implementerar en prioritetskö med ett
 * fält med konstant storlek. Implementeringen gör det lätt
 * att sätta in nya element med svårt att plocka ut det största.
 *
 * Niclas Åstrand, Data- och programstrukturer HT-2003
 * Senast modifierad: 23/7 - 2003
 */

#include "pqueue.h"
#include "genlib.h"

/* Constant: MAX_ELEMENTS
 * ----------------------
 * Den här konstanten anger antalet element det fält som
 * utgör representationen av prioritetskön rymmer.
 */

#define MAX_ELEMENTS 1200000

struct pqueueCDT {
	int entries[MAX_ELEMENTS];
	int numEntries;
};

/* Exported endries */

pqueueADT NewPQueue(void)
{
	pqueueADT pqueue;

	pqueue = New(pqueueADT);
	pqueue->numEntries = 0;
	return (pqueue);
}

void FreePQueue(pqueueADT pqueue)
{
	FreeBlock(pqueue);
}

bool IsEmpty(pqueueADT pqueue)
{
	return (pqueue->numEntries == 0);
}

bool IsFull(pqueueADT pqueue)
{
	return (pqueue->numEntries == MAX_ELEMENTS);
}

/*
 * Implementation notes: Enqueue
 * -----------------------------
 * Då elementen sparas osorterat i fältet behöver endast nya
 * elementet placeras i slutet av fältet.
 */

void Enqueue(pqueueADT pqueue, int newValue)
{
	if (IsFull(pqueue))
		Error("Tried to enqueue into a priority queue which is full!");
	pqueue->entries[pqueue->numEntries++] = newValue;
}

/*
 * Implementation notes: DequeueMax
 * -------------------------------------------------
 * Då elementen sparas osorterat i fältet måste en sökning göras
 * för att finna det största elementet. För att ta bort det största
 * elementet flyttas det sista elementet i fältet till den position
 * i vilken det största elementet återfanns samtidigt som antalet
 * element i fältet minskas med 1. Det finns ingen anledning att skifta
 * samtliga element ett steg åt vänster för att täcka det gap som uppstår
 * vid borttagning av ett element. Det värde som tas bort returneras från
 * funktionen.
 */

int DequeueMax(pqueueADT pqueue)
{
	int maxIndex, value, i;

	if (IsEmpty(pqueue))
		Error("Tried to dequeue max from an empty pqueue!");

	maxIndex = 0;
	for (i = 1; i < pqueue->numEntries; i++) {
		if (pqueue->entries[i] > pqueue->entries[maxIndex])
			maxIndex = i;
	}
	/* spara värdet som skall returneras */
	value = pqueue->entries[maxIndex];
	/* flytta det sista värdet i fältet hit */
	pqueue->entries[maxIndex] = pqueue->entries[--(pqueue->numEntries)];
	return (value);
}

int BytesUsed(pqueueADT pqueue)
{
	return (sizeof(*pqueue));
}

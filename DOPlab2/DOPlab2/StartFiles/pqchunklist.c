/*
 * File: pqchunklist.c
 * ------------------
 */

#include "pqueue.h"
#include "genlib.h"
#include <stdio.h>

/* Definitions */

#define MAX_ELEMS_PER_BLOCK 4

/* Types */

typedef struct chunkT {
	int numValues;
	int value[MAX_ELEMS_PER_BLOCK];
	struct chunkT *next;
} chunkT;

struct pqueueCDT {
	chunkT *head;	
};

/* Exported entries */

pqueueADT NewPQueue(void)
{
	pqueueADT pqueue;

	pqueue = New(pqueueADT);
	pqueue->head = NULL;
	return (pqueue);
}

void FreePQueue(pqueueADT pqueue)
{
	chunkT *next;

	while (pqueue->head != NULL) {
    	next = pqueue->head->next;
    	FreeBlock(pqueue->head);
    	pqueue->head = next;
    }
	FreeBlock(pqueue);
}

bool IsEmpty(pqueueADT pqueue)
{
	return (pqueue->head == NULL);
}

bool IsFull(pqueueADT pqueue)
{
	return (FALSE);
}

/* Implementation notes: Enqueue
 * -----------------------------
 * F�r att finna r�tt position f�r ins�ttning m�ste en s�kning utf�ras.
 * S�kningar i enkell�nkade listor kan vara 'kn�ligt'. I den h�r
 * implementeringen anv�nds tv� 'paralella pekare, en f�ljer ett steg
 * bakom den andra, till dess att vi finner korrekt position f�r ins�ttning
 * av den nya noden. Notera specialfallet vid ins�ttning vid listans
 * huvud.
 */

void Enqueue(pqueueADT pqueue, int newValue)
{
	chunkT *cur, *prev, *newChunk;
 	
	newChunk = New(chunkT *);
 	newChunk->value[0] = newValue;
 	
 	for (prev = NULL, cur = pqueue->head; cur != NULL; prev = cur, cur = cur->next) {
 		if (newValue > cur->value) break;
 	}
 	newChunk->next = cur;
 	if (prev)
 		prev->next = newChunk;
 	else
 		pqueue->head = newChunk;
}

/* Implementation notes: DequeueMax
 * --------------------------------
 * Det st�rsta v�rdet sparas f�rst i f�rsta chunken s� att det �r
 * enkelt att ta bort. Notera att minne f�r noder frig�rs endast
 * vid f�rekomsten av en tom chunk.
 */

int DequeueMax(pqueueADT pqueue)
{
	chunkT *toBeDeleted;
	int value;
	int i = 0;
	if (IsEmpty(pqueue))
		Error("Tried to dequeue max from an empty pqueue!");

 	toBeDeleted = pqueue->head;
	value = pqueue->head->value[0];
	pqueue->head->numValues--;	//Minska antal v�rden i chunken med ett
	
	if (pqueue->head->numValues == 0){ //Om inga v�rden finns kvar i chunken, tar vi bort den och pekar p� n�sta
		pqueue->head = pqueue->head->next;
		FreeBlock(toBeDeleted);
	}
	return (value);
}

/* Implementation notes: BytesUsed
 * -------------------------------
 * Minnes f�rbrukningen utg�rs av minnet f�r en struct pqueueCDT +
 * storleken p� summan av samtliga chunks i chunklistan.
 */

int BytesUsed(pqueueADT pqueue)
{
	int total;
	chunkT *cur;

	total = sizeof(*pqueue);
	for (cur = pqueue->head; cur != NULL; cur = cur->next)
 		total += sizeof(*cur);

	return (total);
}

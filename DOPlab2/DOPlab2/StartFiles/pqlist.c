/*
 * File: pqlist.c
 * ------------------
 *
 * Niclas Åstrand, Data- och programstrukturer HT-2003
 * Senast modifierad: 23/7 - 2003
 */

#include "pqueue.h"
#include "genlib.h"

/* Types */

typedef struct cellT {
	int value;
	struct cellT *next;
} cellT;

struct pqueueCDT {
	cellT *head;	
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
	cellT *next;

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
 * För att finna rätt position för insättning måste en sökning utföras.
 * Sökningar i enkellänkade listor kan vara 'knöligt'. I den här
 * implementeringen används två 'paralella pekare, en följer ett steg
 * bakom den andra, till dess att vi finner korrekt position för insättning
 * av den nya noden. Notera specialfallet vid insättning vid listans
 * huvud.
 */

void Enqueue(pqueueADT pqueue, int newValue)
{
	cellT *cur, *prev, *newCell;
 	
	newCell = New(cellT *);
 	newCell->value = newValue;
 	
 	for (prev = NULL, cur = pqueue->head; cur != NULL; prev = cur, cur = cur->next) {
 		if (newValue > cur->value) break;
 	}
 	newCell->next = cur;
 	if (prev)
 		prev->next = newCell;
 	else
 		pqueue->head = newCell;
}

/* Implementation notes: DequeueMax
 * --------------------------------
 * Det största värdet sparas först i listan så att det är
 * enkelt att ta bort. Notera att minne för noder frigörs
 * vid borttagning ur kön.
 */

int DequeueMax(pqueueADT pqueue)
{
	cellT *toBeDeleted;
	int value;

	if (IsEmpty(pqueue))
		Error("Tried to dequeue max from an empty pqueue!");

 	toBeDeleted = pqueue->head;
 	value = pqueue->head->value;
 	pqueue->head = pqueue->head->next; 	
	FreeBlock(toBeDeleted);
	return (value);
}

/* Implementation notes: BytesUsed
 * -------------------------------
 * Minnes förbrukningen utgörs av minnet för en struct pqueueCDT +
 * storleken på summan av samtliga noder i den länkade listan.
 */

int BytesUsed(pqueueADT pqueue)
{
	int total;
	cellT *cur;

	total = sizeof(*pqueue);
	for (cur = pqueue->head; cur != NULL; cur = cur->next)
 		total += sizeof(*cur);

	return (total);
}

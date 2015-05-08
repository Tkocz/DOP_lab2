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
* För att finna rätt position för insättning måste en sökning utföras.
* Sökningar i enkellänkade chunk-listor kan vara 'knöligt'. I den här
* implementeringen används två 'paralella pekare, en följer ett steg
* bakom den andra, till dess att vi finner korrekt chunk för insättning
* av den nya noden. Om insättning kräver att chunken splittas, görs detta,
* och ett nytt anrop följer. Notera specialfallet vid insättning vid listans
* huvud.
*/

void Enqueue(pqueueADT pqueue, int newValue)
{
	chunkT *cur, *prev, *newChunk;
	int i = 0;

	for (prev = NULL, cur = pqueue->head; cur != NULL; prev = cur, cur = cur->next) {
		if (newValue > cur->value[0] || newValue >= cur->value[cur->numValues - 1]) break;
	}

	if (cur->numValues < MAX_ELEMS_PER_BLOCK){
		while (newValue <= cur->value[i]) i++;
		for (int j = MAX_ELEMS_PER_BLOCK; j > i; j--){
			cur->value[j] = cur->value[j - 1];
		}
		cur->value[i] = newValue;
		cur->numValues++;
	}

	else if (cur->numValues == MAX_ELEMS_PER_BLOCK){

		newChunk = New(chunkT *);
		newChunk->next = cur;
		if (prev){
			newChunk->next = prev->next;
			prev->next = newChunk;
		}
		else{
			newChunk->next = pqueue->head->next;
			pqueue->head->next = newChunk;
		}

		for (int i = 0; i < MAX_ELEMS_PER_BLOCK / 2; i++){		//Flytta den sista hälften i den fulla chunken till en ny, efterföljande chunk
			newChunk->value[i] = cur->value[i + MAX_ELEMS_PER_BLOCK / 2];
			cur->value[i] = cur->value[i + MAX_ELEMS_PER_BLOCK / 2];
			cur->value[i + MAX_ELEMS_PER_BLOCK / 2] = NULL;
			cur->numValues--;
			newChunk->numValues++;
		}
		Enqueue(pqueue, newValue);
	}

}

/* Implementation notes: DequeueMax
* --------------------------------
* Det största värdet sparas först i första chunken så att det är
* enkelt att ta bort. Notera att minne för noder frigörs endast
* vid förekomsten av en tom chunk.
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
	pqueue->head->numValues--;	//Minska antal värden i chunken med ett

	if (pqueue->head->numValues == 0){ //Om inga värden finns kvar i chunken, tar vi bort den och pekar på nästa
		pqueue->head = pqueue->head->next;
		FreeBlock(toBeDeleted);
	}
	return (value);
}

/* Implementation notes: BytesUsed
* -------------------------------
* Minnes förbrukningen utgörs av minnet för en struct pqueueCDT +
* storleken på summan av samtliga chunks i chunklistan.
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

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
* S�kningar i enkell�nkade chunk-listor kan vara 'kn�ligt'. I den h�r
* implementeringen anv�nds tv� 'paralella pekare, en f�ljer ett steg
* bakom den andra, till dess att vi finner korrekt chunk f�r ins�ttning
* av den nya noden. Om ins�ttning kr�ver att chunken splittas, g�rs detta,
* och ett nytt anrop f�ljer. Notera specialfallet vid ins�ttning vid listans
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

		for (int i = 0; i < MAX_ELEMS_PER_BLOCK / 2; i++){		//Flytta den sista h�lften i den fulla chunken till en ny, efterf�ljande chunk
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

/*
* File: pqlistchunklist.c
* ------------------
*
* Henrik Stridsman & Martin Bergqvist, Data- och programstrukturer VT-2015
* Senast modifierad: 11/5 - 2015
*/
#include "pqueue.h"
#include "genlib.h"

/* Prototypes */
pqueueADT NewPQueue(void);
void FreePQueue(pqueueADT pqueue);
bool IsEmpty(pqueueADT pqueue);
bool IsFull(pqueueADT pqueue);
void Enqueue(pqueueADT pqueue, int newValue);
int DequeueMax(pqueueADT pqueue);
int cmpfunc(const void * a, const void * b);

/* Predefines */
#define MAX_ELEMS_PER_BLOCK 6

/* Types */
typedef struct chunkT{

	int					chunkSize;
	int					values[MAX_ELEMS_PER_BLOCK];
	struct chunkT		*nextChunk;

}chunkT;

typedef struct pqueueCDT {

	int		numEntries, numChunks;
	chunkT	*head;

}pqueueCDT;

/* Exported entries */
pqueueADT NewPQueue(void){

	pqueueADT pqueue;

	pqueue = New(pqueueADT);
	pqueue->head = NULL;
	pqueue->numEntries = 0;
	pqueue->numChunks = 0;

	return pqueue;
}

void FreePQueue(pqueueADT pqueue){

	FreeBlock(pqueue);
}

bool IsEmpty(pqueueADT pqueue){

	return (pqueue->numEntries == 0);
}

bool IsFull(pqueueADT pqueue)
{
	return (FALSE);
}

/* Implementation notes: Enqueue
* -----------------------------
* För att finna rätt position jämförs insert-värdet med första värdet i 
* nästa chunk. Är värdet större, så sätts värdet in i nuvarande chunk,
* på sista position, och sorteras på plats med qsort. Skulle chunken vara full,
* splittas den till två chunks, med hälften av värdena i vardera chunk, och Enqueueu
* åberopas på nytt.
*/
void Enqueue(pqueueADT pqueue, int newValue){

	chunkT	*newChunk, *tempChunk;

	tempChunk = pqueue->head;

	if (IsEmpty(pqueue)){

		newChunk = New(chunkT *);
		int i;

		for (i = 0; i < MAX_ELEMS_PER_BLOCK; i++){
			newChunk->values[i] = NULL;
		}

		newChunk->chunkSize = 0;
		newChunk->nextChunk = NULL;
		pqueue->numChunks++;

		newChunk->values[0] = newValue;
		newChunk->chunkSize++;
		pqueue->numEntries++;
		pqueue->head = newChunk;
	}

	else{

		while (tempChunk->nextChunk != NULL && tempChunk->nextChunk->values[0] > newValue){
			tempChunk = tempChunk->nextChunk;
		}
		if (tempChunk->chunkSize == MAX_ELEMS_PER_BLOCK){

			newChunk = New(chunkT *);
			int i;

			for (i = 0; i < MAX_ELEMS_PER_BLOCK; i++){
				newChunk->values[i] = NULL;
			}

			newChunk->chunkSize = 0;
			newChunk->nextChunk = NULL;
			pqueue->numChunks++;
			
			for (i = 0; i < MAX_ELEMS_PER_BLOCK / 2; i++){
				newChunk->values[i] = tempChunk->values[i + MAX_ELEMS_PER_BLOCK / 2];
				tempChunk->values[i + MAX_ELEMS_PER_BLOCK / 2] = NULL;
				newChunk->chunkSize++;
				tempChunk->chunkSize--;
			}

			newChunk->nextChunk = tempChunk->nextChunk;
			tempChunk->nextChunk = newChunk;
			Enqueue(pqueue, newValue);
			return;
		}
		else{
			tempChunk->values[tempChunk->chunkSize] = newValue;
			tempChunk->chunkSize++;
			pqueue->numEntries++;

			qsort(tempChunk->values, tempChunk->chunkSize, sizeof(int), cmpfunc);
		}
	}
}

/* Implementation notes: DequeueMax
* --------------------------------
* Det största värdet sparas först i chunklistan så att det är
* enkelt att ta bort. Notera att minne för chunks frigörs
* vid borttagning ur kön, om det innebär att chunken blir tom.
*/
int DequeueMax(pqueueADT pqueue){

	chunkT *tempChunk = pqueue->head;

	int value;
	int i;

	if (IsEmpty(pqueue))
		Error("Tried to dequeue max from an empty pqueue!");

	value = pqueue->head->values[0];
	for (i = 0; i < tempChunk->chunkSize - 1; i++){
		if (pqueue->head->values[i] == NULL){
			break;
		}
		pqueue->head->values[i] = pqueue->head->values[i + 1];
	}
	pqueue->head->chunkSize--;		//Minska antal värden i chunken med ett
	pqueue->numEntries--;			//Minska antal värden i chunklistan med ett

	if (tempChunk->chunkSize == 0){ //Om inga värden finns kvar i chunken, tar vi bort den och pekar på nästa
		pqueue->head = pqueue->head->nextChunk;
		FreeBlock(tempChunk);
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
	for (cur = pqueue->head; cur != NULL; cur = cur->nextChunk)
		total += sizeof(*cur);

	return (total);
}
/* Implementation notes: cmpfunc
* -------------------------------
* Hjälpfunktion för qsort.
*/
int cmpfunc(const void * a, const void * b)
{
	return (*(int*)b - *(int*)a);
}
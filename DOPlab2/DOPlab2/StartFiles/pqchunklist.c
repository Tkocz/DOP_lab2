#include "pqueue.h"
#include "genlib.h"

pqueueADT NewPQueue(void);
void FreePQueue(pqueueADT pqueue);
bool IsEmpty(pqueueADT pqueue);
bool IsFull(pqueueADT pqueue);
bool IsChunkFull(pqueueADT pqueue);
void Enqueue(pqueueADT pqueue, int newValue);
int DequeueMax(pqueueADT pqueue);

#define MAX_ELEMS_PER_BLOCK 6

typedef struct chunk{

	int					chunkSize;
	int					values[MAX_ELEMS_PER_BLOCK];
	struct chunk		*nextChunk;

}chunk;

typedef struct pqueueCDT {

	int		numEntries, numChunks;
	chunk	*head;


}pqueueCDT;

pqueueADT NewPQueue(void){

	pqueueADT pqueue;


	pqueue = New(pqueueADT);
	pqueue->head = NULL;
	pqueue->numEntries = 0;
	pqueue->numChunks = 0;

	return pqueue;
}

chunk NewChunk(pqueueADT pqueue){

	chunk *newChunk;

	newChunk = New(chunk *);
	int i;

	for (i = 0; i < MAX_ELEMS_PER_BLOCK; i++){
		newChunk->values[i] = NULL;
	}

	newChunk->chunkSize = 0;
	newChunk->nextChunk = NULL;
	pqueue->numChunks++;

	return *newChunk;
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

bool IsChunkFull(pqueueADT pqueue){

	int i;
	chunk *tempChunk = pqueue->head;

	for (i = 0; i < pqueue->numChunks; i++){
		if (tempChunk->chunkSize == MAX_ELEMS_PER_BLOCK)
			return TRUE;
		else
			tempChunk = tempChunk->nextChunk;

		if (tempChunk == NULL)
			break;
	}

	return FALSE;
}

void Enqueue(pqueueADT pqueue, int newValue){

	chunk	*newChunk, *tempChunk;
	int		temp;
	int		i, j;


	tempChunk = pqueue->head;


	if (IsEmpty(pqueue)){

		newChunk = New(chunk *);
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
	else if (IsChunkFull(pqueue)){

		newChunk = New(chunk *);
		int i;

		for (i = 0; i < MAX_ELEMS_PER_BLOCK; i++){
			newChunk->values[i] = NULL;
		}

		newChunk->chunkSize = 0;
		newChunk->nextChunk = NULL;
		pqueue->numChunks++;

		while (tempChunk->chunkSize != MAX_ELEMS_PER_BLOCK){
			tempChunk = tempChunk->nextChunk;
		}

		for (i = 0; i < MAX_ELEMS_PER_BLOCK / 2; i++){
			newChunk->values[i] = tempChunk->values[i + MAX_ELEMS_PER_BLOCK / 2];
			tempChunk->values[i + MAX_ELEMS_PER_BLOCK / 2] = NULL;
			newChunk->chunkSize++;
			tempChunk->chunkSize--;
		}

		newChunk->nextChunk = tempChunk->nextChunk;
		tempChunk->nextChunk = newChunk;
		Enqueue(pqueue, newValue);

	}
	else{

		while (tempChunk->nextChunk != NULL && tempChunk->nextChunk->values[0] > newValue){
			tempChunk = tempChunk->nextChunk;
		}

		tempChunk->values[tempChunk->chunkSize] = newValue;
		
		for (i = 0; i < (MAX_ELEMS_PER_BLOCK-1); i++){
			for (j = 0; (j < MAX_ELEMS_PER_BLOCK - 1); j++){
				if (tempChunk->values[j] < tempChunk->values[j + 1]){
					temp = tempChunk->values[j];
					tempChunk->values[j] = tempChunk->values[j + 1];
					tempChunk->values[j + 1] = temp;
				}
			}	
		}
		tempChunk->chunkSize++;
		pqueue->numEntries++;
		
	}

	
}

int DequeueMax(pqueueADT pqueue){

	chunk *tempChunk = pqueue->head;

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
	pqueue->head->chunkSize--;
	pqueue->numEntries--;	//Minska antal värden i chunken med ett

	if (tempChunk->chunkSize == 0){ //Om inga värden finns kvar i chunken, tar vi bort den och pekar på nästa
		pqueue->head = pqueue->head->nextChunk;
		FreeBlock(tempChunk);
	}

	return (value);
}

int BytesUsed(pqueueADT pqueue)
{
	return (sizeof(*pqueue));
}

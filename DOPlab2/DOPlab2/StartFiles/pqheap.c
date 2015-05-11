/*
* File: pqheap.c
* ------------------
*
*Martin Bergqvist & Henrik Stridsman, Data- och programstrukturer VT-2015
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
void heapify(pqueueADT pqueue, int nodeIndex);
void bubbleUp(pqueueADT pqueue, int nodeIndex);

/* Constant: STARTING_ELEMENTS
* ----------------------
* Den h�r konstanten anger antalet element det f�lt som
* utg�r representationen av prioritetsk�n rymmer.
*/
#define STARTING_ELEMENTS 10

/* Types */
struct pqueueCDT {
	int numEntries;
	int *values;
	int curSize;
};

/* Exported entries */

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
	return FALSE;
}
//

/* Implementation notes: Enqueue
* -----------------------------
* F�r att finna r�tt position s�tts insertv�rdet in p� sista position,
* varefter bubbleUp anropas f�r att j�mf�ra v�rdet med sin f�r�lder,
* och bubbla upp om v�rdet �r st�rre.
* Om arrayen �r full, skapas en ny array av dubbel storlek, v�rdena f�rs �ver,
* och den gamla arrayen frig�rs.
*/
void Enqueue(pqueueADT pqueue, int newValue)
{
	if (IsEmpty(pqueue)){
		pqueue->values = NewArray(STARTING_ELEMENTS + 1, int);
		pqueue->curSize = STARTING_ELEMENTS;
		pqueue->values[1] = newValue;
		pqueue->numEntries = 1;
	}

	else if (pqueue->numEntries == pqueue->curSize){
		int *tempArray;
		tempArray = pqueue->values;
		pqueue->values = NewArray(pqueue->curSize * 2 + 1, int);
		pqueue->curSize = (pqueue->curSize * 2);

		for (int i = 1; i <= pqueue->numEntries; i++){
			pqueue->values[i] = tempArray[i];
		}
		FreeBlock(tempArray);
		Enqueue(pqueue, newValue);
		return;
	}
	else {
		pqueue->numEntries++;
		pqueue->values[pqueue->numEntries] = newValue;
		bubbleUp(pqueue, pqueue->numEntries);

	}
}

/* Implementation notes: DequeueMax
* --------------------------------
* Det st�rsta v�rdet sparas �verst i heapen s� att det �r
* enkelt att ta bort. Det ers�tts med sista noden,
* och d�refter anropas heapify f�r att f�ra det ner till
* r�tt position.
*/
int DequeueMax(pqueueADT pqueue)
{
	if (IsEmpty(pqueue))
		Error("Tried to dequeue max from an empty pqueue!");

	int value, i = 1;

	value = pqueue->values[1];
	pqueue->values[1] = pqueue->values[pqueue->numEntries];
	pqueue->values[pqueue->numEntries] = NULL;
	pqueue->numEntries--;
	heapify(pqueue, 1);
	return value;
}

/* Implementation notes: BytesUsed
* -------------------------------
* Minnes f�rbrukningen utg�rs av minnet f�r
* arrayen som representerar heapen.
*/
int BytesUsed(pqueueADT pqueue)
{
	return (sizeof(int) * pqueue->curSize);
}

/* Implementation notes: bubbleUp
* -----------------------------
* J�mf�r ett element med sin f�r�lder rekursivt, och byter plats om barnet �r st�rre.
*/
void bubbleUp(pqueueADT pqueue, int nodeIndex) {

	int parentIndex, tmp;

	if (nodeIndex > 1) {
		parentIndex = nodeIndex / 2;

		if (pqueue->values[parentIndex] <= pqueue->values[nodeIndex]){
			tmp = pqueue->values[parentIndex];
			pqueue->values[parentIndex] = pqueue->values[nodeIndex];
			pqueue->values[nodeIndex] = tmp;
			bubbleUp(pqueue, parentIndex);
		}
	}
}

/* Implementation notes: heapify
* -----------------------------
* J�mf�r ett element med sina barn, och byter plats, rekursivt,
* med det st�rsta barnet, om det �r st�rre �n elementet.
*/
void heapify(pqueueADT pqueue, int nodeIndex) {

	int leftChild, rightChild, maxIndex, tmp;

	leftChild = nodeIndex * 2;
	rightChild = nodeIndex * 2 + 1;

	if (leftChild > pqueue->numEntries)return;

	if ((pqueue->values[leftChild] >= pqueue->values[rightChild]) || rightChild > pqueue->numEntries)
		maxIndex = leftChild;
	else
		maxIndex = rightChild;

	if (pqueue->values[nodeIndex] <= pqueue->values[maxIndex]) {
		tmp = pqueue->values[maxIndex];
		pqueue->values[maxIndex] = pqueue->values[nodeIndex];
		pqueue->values[nodeIndex] = tmp;
		heapify(pqueue, maxIndex);
	}
}
#include "pqueue.h"
#include "genlib.h"

struct pqueueCDT {
	int numEntries;
	int values[10000000];
	int curSize;
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
	return FALSE;
}

void Enqueue(pqueueADT pqueue, int newValue)
{
	int temp, i;
	int *tempArray;

	if (IsEmpty(pqueue)){
		//pqueue->values = NewArray(sizeof(int) * 2, int);
		pqueue->curSize = sizeof(int) * 2;
		pqueue->values[1] = newValue;
		pqueue->values[0] = 99999999;
	}
	else{
		pqueue->values[pqueue->numEntries + 1] = newValue;
		while (pqueue->values[pqueue->numEntries / 2] < newValue){
			temp = pqueue->values[pqueue->numEntries / 2];
			pqueue->values[pqueue->numEntries / 2] = pqueue->values[pqueue->numEntries + 1];
			pqueue->values[pqueue->numEntries + 1] = temp;
		}
		

	}

	if (pqueue->numEntries == (pqueue->curSize - 3)){
		tempArray = pqueue->values;
		//pqueue->values = NewArray(sizeof(int) * (pqueue->curSize * 2), int);
		pqueue->curSize = sizeof(int) * (pqueue->curSize * 2);

		for (i = 1; i < pqueue->numEntries + 1; i++){
			pqueue->values[i] = tempArray[i];
		}
	}
	
	pqueue->numEntries++;

}


int DequeueMax(pqueueADT pqueue)
{
	int value, i = 1, temp;

	value = pqueue->values[1];
	pqueue->values[1] = pqueue->values[pqueue->numEntries + 1];
	pqueue->values[pqueue->numEntries + 1] = NULL;

	while (pqueue->values[i] < pqueue->values[i * 2] || pqueue->values[i] < pqueue->values[i * 2 + 1] && pqueue->values[i * 2 + 1] != NULL) {

		if (pqueue->values[i * 2] > pqueue->values[i * 2 + 1]){
			temp = pqueue->values[i * 2];
			pqueue->values[i * 2] = pqueue->values[i];
			pqueue->values[i] = temp;
		}
		else{
			temp = pqueue->values[i * 2 + 1];
			pqueue->values[i * 2 + 1] = pqueue->values[i];
			pqueue->values[i] = temp;
		}
		i++;
	}
	pqueue->numEntries--;

	return value;
}

int BytesUsed(pqueueADT pqueue)
{
	return (sizeof(*pqueue));
}

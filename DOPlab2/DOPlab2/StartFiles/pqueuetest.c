/*
 * File: pqueuetest.c
 * ------------------
 *
 * Niclas Åstrand, Data- och programstrukturer HT-2003
 * Senast modifierad: 23/7 - 2003
 */

#include <stdio.h>
#include "genlib.h"
#include "pqueue.h"
#include "pqueuetest.h"
#include "simpio.h"
#include "random.h"

static bool ArrayIsSorted(int array[], int size);

void HeapSort(pqueueADT pq, int array[], int nElements)
{
	int i;

	for (i = 0; i < nElements; i++)
		Enqueue(pq, array[i]);
	for (i = 0; i < nElements; i++)
		array[nElements-i-1] = DequeueMax(pq);
}

void BasicPQueueTest(void)
{
	int i;
	pqueueADT pq;

	printf("\n-----------   Testing Basic PQueue functions -----------\n\n");
	pq = NewPQueue();
	printf("The pqueue was just created.  Is it empty? %s", IsEmpty(pq) ? "TRUE" : "FALSE");

	for (i = 1; i <= 10; i++)
		Enqueue(pq, i);

	printf("\nEnqueuing the integers from 1 to 10 (in forward order)\n");
	printf("Pqueue should not be empty. Is it empty? %s\n", IsEmpty(pq) ? "TRUE" : "FALSE");
	printf("Dequeuing the top 5 elements: ");

	for (i = 0; i < 5; i++)
		printf("%d ", DequeueMax(pq));

	printf("\nDequeuing all the rest: ");
	while (!IsEmpty(pq))
		printf("%d ", DequeueMax(pq));

	printf("\nPqueue should be empty.  Is it empty? %s\n", IsEmpty(pq) ? "TRUE" : "FALSE");

	FreePQueue(pq);
	printf("Hit return to continue: ");
	{
		string s = GetLine();
		FreeBlock(s);
	}
}

void MorePQueueTest(void)
{
	int i;
	pqueueADT pq;

	printf("\n-----------   More pqueue testing functions -----------\n");
	pq = NewPQueue();

	printf("\nEnqueuing the integers from 1 to 15 (in reverse order)\n");
	for (i = 15; i > 0; i--)
		Enqueue(pq, i);

	printf("Enqueuing duplicates for evens from 2 to 14\n");
	for (i = 2; i <= 14; i += 2)
		Enqueue(pq, i);

	printf("Dequeuing the top 10 elements: ");
	for (i = 0; i < 10; i++)
		printf("%d ", DequeueMax(pq));

	printf("\nDequeuing all the rest: ");
	while (!IsEmpty(pq))
		printf("%d ", DequeueMax(pq));

	printf("\nPQueue should be empty. Is it empty? %s", IsEmpty(pq) ? "TRUE" : "FALSE");

	printf("\nThis next test raises an error if your pqueue is working correctly.\n");
	printf("Once you verify the test, comment it out to move on to the other tests.\n");
	printf("(The test to comment out is line %d in the file %s).\n", __LINE__ + 1,  __FILE__);
	//printf("Dequeue from empty pqueue returns %d", DequeueMax(pq));

	FreePQueue(pq);
	printf("Hit return to continue: ");
	{
		string s = GetLine();
		FreeBlock(s);
	}
}

#define SORT_SIZE 5000

void PQueueSortTest(void)
{
	int i;
	pqueueADT pq;
	int array[SORT_SIZE];

	printf("\n-----------   Testing use of pqueue to sort  -----------\n");
	pq = NewPQueue();

	printf("Enqueuing %d numbers into pqueue in increasing order.", SORT_SIZE);
	for (i = 0; i < SORT_SIZE; i++) array[i] = i;
	HeapSort(pq, array, SORT_SIZE);
	printf("\nUsing dequeue to pull out numbers in sorted order.  Are they sorted? %s\n",
		ArrayIsSorted(array, SORT_SIZE) ? "TRUE" : "FALSE");
        printf("PQueue should be empty. Is it empty? %s\n", IsEmpty(pq) ? "TRUE" : "FALSE");

	printf("\nEnqueuing %d numbers into pqueue in decreasing order.", SORT_SIZE);
	for (i = 0; i < SORT_SIZE; i++) array[i] = SORT_SIZE - i;
	HeapSort(pq, array, SORT_SIZE);
	printf("\nUsing dequeue to pull out numbers in sorted order.  Are they sorted? %s\n",
		ArrayIsSorted(array, SORT_SIZE) ? "TRUE" : "FALSE");
        printf("PQueue should be empty. Is it empty? %s\n", IsEmpty(pq) ? "TRUE" : "FALSE");

	printf("\nEnqueuing %d random values into the pqueue.\n", SORT_SIZE);
        for (i = 0; i < SORT_SIZE; i++) array[i] = RandomInteger(1, 1000);
	HeapSort(pq, array, SORT_SIZE);
	printf("Using dequeue to pull out numbers in sorted order.  Are they sorted? %s\n",
		ArrayIsSorted(array, SORT_SIZE) ? "TRUE" : "FALSE");
        printf("PQueue should be empty. Is it empty? %s\n", IsEmpty(pq) ? "TRUE" : "FALSE");

	printf("\nEnqueuing %d random possibly negative values into the pqueue.\n", SORT_SIZE);
        for (i = 0; i < SORT_SIZE; i++) array[i] = RandomInteger(-1000, 1000);
	HeapSort(pq, array, SORT_SIZE);
	printf("Using dequeue to pull out numbers in sorted order.  Are they sorted? %s\n",
		ArrayIsSorted(array, SORT_SIZE) ? "TRUE" : "FALSE");
        printf("PQueue should be empty. Is it empty? %s\n", IsEmpty(pq) ? "TRUE" : "FALSE");

	FreePQueue(pq);
	printf("Hit return to continue: ");
	{
		string s = GetLine();
		FreeBlock(s);
	}
}


static bool ArrayIsSorted(int array[], int size)
{
	int i;

	for (i = 0; i < size - 1; i++)
		if (array[i] > array[i + 1]) return (FALSE);
	return (TRUE);
}


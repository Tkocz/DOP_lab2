/*
 * File: pqueue.h
 * --------------
 * Gränssnitt för abstrakt datatyp pqueueADT
 *
 * Niclas Åstrand, Data- och programstrukturer HT-2003
 * Senast modifierad: 23/7 - 2003
 */

#ifndef _pqueue_h
#define _pqueue_h

#include "genlib.h"

/*
 * Type: pqueueADT
 * ---------------
 * Det här är en abstrakt datatyp som representerar en
 * prioritetskö. En prioritetskö är inte en vanligt FIFO-kö
 * utan en kö där elementen plockas ut i en prioritetsordning.
 * Elementen i prioritetskön är heltal och representerar
 * prioriteten (större heltal har högre prioritet).
 */

typedef struct pqueueCDT *pqueueADT;

/*
 * Function: NewPQueue
 * Usage: pqueue = NewPQueue();
 * ----------------------------
 * Skapar en tom prioritetskö.
 */

pqueueADT NewPQueue(void);

/*
 * Function: FreePQueue
 * Usage: FreePQueue(pqueue);
 * -------------------------
 * Frigör minnet associerat med en prioritetskö.
 */

void FreePQueue(pqueueADT pqueue);

/*
 * Function: IsEmpty
 * Usage: if (IsEmpty(pqueue)) . . .
 * ---------------------------------
 * Returnerar TRUE om prioritetskön är tom.
 */

bool IsEmpty(pqueueADT pqueue);

/*
 * Function: IsFull
 * Usage: if (IsFull(pqueue)) . . .
 * ---------------------------------
 * Returnerar TRUE om prioritetskön inte har plats för
 * ytterligare element. Klienter kan använda den här funtionen
 * för att kontrollera om ytterligare insättningar kommer medföra
 * att prioritetköns kapacitet överstigs. Notera att somliga
 * implementeringar alltid returnerar FALSE.
 */

bool IsFull(pqueueADT pqueue);

/*
 * Function: Enqueue
 * Usage: Enqueue(pqueueADT pqueue, int newValue);
 * -----------------------------------------------
 * Insättning av angivet värde (newValue) i prioritetskön. Om prioritetskön
 * är full kommer funktionen att rapportera detta.
 */

void Enqueue(pqueueADT pqueue, int newValue);

/*
 * Function: DequeueMax
 * Usage: maxElement = DequeueMax(pqueueADT pqueue);
 * -------------------------------------------------
 * Tar bort, och returnerar, elementet med högst prioritet i kön.
 * Om prioritetskön är tom kommer funktionen att rapportera detta.
 */

int DequeueMax(pqueueADT pqueue);

/*
 * Function: BytesUsed
 * Usage: size = BytesUsed(pqueueADT pqueue);
 * ------------------------------------------
 * Givet en prioritetskö så beräknar den här funktionen, och returnerar,
 * det totala antalet bytes kön förbrukar.
 */

int BytesUsed(pqueueADT pqueue);

#endif

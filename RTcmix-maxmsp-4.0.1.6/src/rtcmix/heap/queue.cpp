/* RTcmix  - Copyright (C) 2000  The RTcmix Development Team
   See ``AUTHORS'' for a list of contributors. See ``LICENSE'' for
   the license to this software and for a DISCLAIMER OF ALL WARRANTIES.
*/
#include <iostream>
#include "heap.h"

using namespace std;

queue::~queue()
{
//	cout << "queue::~queue()\n";
	if (tail != head) {
	    delete tail;
	}
	delete head;
// 	while (head) {
// 		qElt *next = head->next;
// 		delete head;
// 		head = next; 
// 	}
}

void 
queue::pushTail(heapslot *newHeapElt)
{
  // create new qElt
  qElt *newElt = new qElt(newHeapElt);

  if (head == NULL)  // if first item on queue
    head = tail = newElt;
  else {  // append to the end of the queue
    tail->next = newElt;
    newElt->prev = tail;
    tail = newElt;
  }
}

void 
queue::push(heapslot *newHeapElt)
{
  qElt *newElt = new qElt(newHeapElt);
  
  if (head == NULL)
    head = tail = newElt;
  else {  // push to front of queue
    newElt->next = head;
    head->prev = newElt;
    head = newElt;
  }
}

heapslot *
queue::pop() 
{
  qElt *tQelt;
  heapslot *retHeap;
  tQelt = head;
  if (!head) {
    cerr << "ERROR: attempt to pop empty queue\n";
    return NULL;
  }
  retHeap = head->heap;
  head = head->next;
  delete tQelt;
  return retHeap;
}

heapslot *
queue::popTail()
{
  qElt *tQelt;
  heapslot *retHeap;
  tQelt = tail;
  if (!tail) {
    cerr << "ERROR: attempt to popTail empty queue\n";
    return NULL;
  }
  retHeap = tail->heap;
  tail = tail->prev;
  delete tQelt;
  return retHeap;
}






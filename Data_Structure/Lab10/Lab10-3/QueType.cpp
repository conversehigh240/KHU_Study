#include <cstddef>                 // For NULL
#include <new>                     // For bad_alloc
#include "QueType.h"

struct NodeType_Q
{
  ItemType info;
  NodeType_Q* next;
};



QueType::QueType()          // Class constructor.
// Post:  front and rear are set to NULL.
{
  front = NULL;
  rear = NULL;
}


void QueType::MakeEmpty()
// Post: Queue is empty; all elements have been deallocated.
{
  NodeType_Q* tempPtr;

    while (front != NULL)
  {
    tempPtr = front;
    front = front->next;
    delete tempPtr;
  }
  rear = NULL;
}

          // Class destructor.
QueType::~QueType()
{
  MakeEmpty();
}


bool QueType::IsFull() const
// Returns true if there is no room for another ItemType 
//  on the free store; false otherwise.
{
  NodeType_Q* location;
  try
  {
    location = new NodeType_Q;
    delete location;
    return false;
  }
  catch(std::bad_alloc)
  {
    return true;
  }
}


bool QueType::IsEmpty() const
// Returns true if there are no elements on the queue; false otherwise.
{
  return (front == NULL);
}


void QueType::Enqueue(ItemType newItem)
// Adds newItem to the rear of the queue.
// Pre:  Queue has been initialized.
// Post: If (queue is not full) newItem is at the rear of the queue;
//       otherwise a FullQueue exception is thrown.  

{
  if (IsFull())
    throw FullQueue();
  else
  {
    NodeType_Q* newNode;

    newNode = new NodeType_Q;
    newNode->info = newItem;
    newNode->next = NULL;
    if (rear == NULL)
      front = newNode;
    else
      rear->next = newNode;
    rear = newNode;
  }
}


void QueType::Dequeue(ItemType& item)
// Removes front item from the queue and returns it in item.
// Pre:  Queue has been initialized and is not empty.
// Post: If (queue is not empty) the front of the queue has been 
//       removed and a copy returned in item; 
//       othersiwe a EmptyQueue exception has been thrown.
{
  if (IsEmpty())
    throw EmptyQueue();
  else
  {
    NodeType_Q* tempPtr;

    tempPtr = front;
    item = front->info;
    front = front->next;
    if (front == NULL)
      rear = NULL;
    delete tempPtr;
  }
}

QueType::QueType
  (const QueType& anotherQue)
{
  NodeType_Q* ptr1;
  NodeType_Q* ptr2;
  
  if (anotherQue.front == NULL)
    front = NULL;
  else
  {
    front = new NodeType_Q;
    front->info = anotherQue.front->info;
    ptr1 = anotherQue.front->next;
    ptr2 = front;
    while (ptr1 != NULL)
    {
      ptr2->next = new NodeType_Q;
      ptr2 = ptr2->next;
      ptr2->info = ptr1->info;
      ptr1 = ptr1->next;
    }
    ptr2->next = NULL;
    rear = ptr2;
  }    
}

// Definition of class PQType, which represents the Priority Queue ADT
class FullPQ{};
class EmptyPQ{};
#include "heap.h"
#include "SortedType.h"
template<class ItemType>
class PQType
{
public:
  PQType();          // parameterized class constructor
  ~PQType();            // class destructor
  
  void MakeEmpty();
  // Function: Initializes the queue to an empty state.
  // Post: Queue is empty.
  
  bool IsEmpty() const;
  // Function: Determines whether the queue is empty.
  // Post: Function value = (queue is empty)
  
  bool IsFull() const;
  // Function: Determines whether the queue is full.
  // Post: Function value = (queue is full)
  
  void Enqueue(ItemType newItem);
  // Function: Adds newItem to the rear of the queue.
  // Post: if (the priority queue is full) exception FullPQ is thrown;
  //       else newItem is in the queue.
  
  void Dequeue(ItemType& item);
  // Function: Removes element with highest priority from the queue
  // and returns it in item.
  // Post: If (the priority queue is empty) exception EmptyPQ is thrown;
  //       else highest priority element has been removed from queue.
  //       item is a copy of removed element.
private:
  int length;
  HeapType<ItemType> items;
  SortedType<ItemType> linkedlist;
};

template<class ItemType>
PQType<ItemType>::PQType()
{
    length = linkedlist.LengthIs();
}

template<class ItemType>
void PQType<ItemType>::MakeEmpty()
{
  length = 0;
}

template<class ItemType>
PQType<ItemType>::~PQType()
{
  linkedlist.MakeEmpty();
}
template<class ItemType>
void PQType<ItemType>::Dequeue(ItemType& item)
// Post: element with highest priority has been removed 
//       from the queue; a copy is returned in item.
{
  if (length == 0)
    throw EmptyPQ();
  else
  {
      linkedlist.ResetList();
      linkedlist.GetNextItem(item);
      linkedlist.DeleteItem(item);
      length--;
  }
}

template<class ItemType>
void PQType<ItemType>::Enqueue(ItemType newItem)
// Post: newItem is in the queue.
{
  if (linkedlist.IsFull())
    throw FullPQ();
  else
  {
      length++;
      linkedlist.InsertItem(newItem);
  }
}
template<class ItemType>
bool PQType<ItemType>::IsFull() const
// Post: Returns true if the queue is full; false, otherwise.
{
  return linkedlist.IsFull();
}

template<class ItemType>
bool PQType<ItemType>::IsEmpty() const
// Post: Returns true if the queue is empty; false, otherwise.
{
  return length == 0;
}

/* 3-D
Heap에서 Enqueue와 Dequeue는 모두 복잡도가 log2(N)이다.
SortedType을 이용한 Linked list에서는 리스트가 내림차순으로 구성되므로
Enqueue의 복잡도는 (N) 이고, Dequeue의 복잡도는 (1)이다.
*/
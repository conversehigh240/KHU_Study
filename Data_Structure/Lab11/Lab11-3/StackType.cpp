// Implementation file for StackType.h

#include <cstddef>
#include <new>
#include "StackType.h"
void StackType::Push(ItemType newItem)
// Adds newItem to the top of the stack.
// Pre:  Stack has been initialized.
// Post: If stack is full, FullStack exception is thrown;
//       else newItem is at the top of the stack.

{
  if (IsFull())
    throw FullStack();
  else
  {
      timestamp++;
      pq.Enqueue(newItem);
  }
}
void StackType::Pop()
// Removes top item from Stack and returns it in item.
// Pre:  Stack has been initialized.
// Post: If stack is empty, EmptyStack exception is thrown;
//       else top element has been removed.
{
  if (IsEmpty())
    throw EmptyStack();
  else
  {
      timestamp--;
  }
}

ItemType StackType::Top()
// Returns a copy of the top item in the stack.
// Pre:  Stack has been initialized.
// Post: If stack is empty, EmptyStack exception is thrown;
//       else a copy of the top element is returned.
{
  if (IsEmpty())
    throw EmptyStack();
  else
  {
      ItemType item;
      pq.Dequeue(item);
      return item;
  }
}

bool StackType::IsEmpty() const
// Returns true if there are no elements on the stack; false otherwise.
{
    return (timestamp == -1);
}

bool StackType::IsFull() const
// Returns true if there is no room for another ItemType 
//  on the free store; false otherwise.
{
    return pq.IsFull();
}

StackType::~StackType()
// Post: stack is empty; all items have been deallocated.
{
    pq.MakeEmpty();
}

StackType::StackType():pq(10)	// Class constructor.
{
    timestamp = -1;
}

/* 4-C
Stack을 이용하여 구현한 우선순위 큐는 Push에 Enqueue가 이루어지고,
이 큐는 Heap으로 구성되어 있으므로 log2(N)의 복잡도이다.
Pop 연산에서도 마찬가지로 가장 큰 수가 먼저 Pop되기 때문에
log2(N)의 복잡도가 필요하다.
*/
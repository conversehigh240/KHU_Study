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
Stack�� �̿��Ͽ� ������ �켱���� ť�� Push�� Enqueue�� �̷������,
�� ť�� Heap���� �����Ǿ� �����Ƿ� log2(N)�� ���⵵�̴�.
Pop ���꿡���� ���������� ���� ū ���� ���� Pop�Ǳ� ������
log2(N)�� ���⵵�� �ʿ��ϴ�.
*/
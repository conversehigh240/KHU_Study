#include "QueType.h"

int Length(QueType& queue);
bool Identical(QueType& queue1, QueType& queue2);
void ReplaceItem(QueType& queue, int oldItem, int newItem);

QueType::QueType(int max)
// Parameterized class constructor
// Post: maxQue, front, and rear have been initialized.
//       The array to hold the queue elements has been dynamically
//       allocated.
{
    maxQue = max + 1;
    front = maxQue - 1;
    rear = maxQue - 1;
    items = new ItemType[maxQue];
    length = 0;
}
QueType::QueType()          // Default class constructor
// Post: maxQue, front, and rear have been initialized.
//       The array to hold the queue elements has been dynamically
//       allocated.
{
    maxQue = 501;
    front = maxQue - 1;
    rear = maxQue - 1;
    items = new ItemType[maxQue];
    length = 0;
}
QueType::~QueType()         // Class destructor
{
    delete[] items;
}

void QueType::MakeEmpty()
// Post: front and rear have been reset to the empty state.
{
    length = 0;
}

bool QueType::IsEmpty() const
// Returns true if the queue is empty; false otherwise.
{
    return (length == 0);
}

bool QueType::IsFull() const
// Returns true if the queue is full; false otherwise.
{
    return (length == maxQue);
}

void QueType::Enqueue(ItemType newItem)
// Post: If (queue is not full) newItem is at the rear of the queue;
//       otherwise a FullQueue exception is thrown.  
{
    if (IsFull())
        throw FullQueue();
    else
    {
        rear = (rear + 1) % maxQue;
        items[rear] = newItem;
        length++;
    }
}

void QueType::Dequeue(ItemType& item)
// Post: If (queue is not empty) the front of the queue has been 
//       removed and a copy returned in item; 
//       othersiwe a EmptyQueue exception has been thrown.
{
    if (IsEmpty())
        throw EmptyQueue();
    else
    {
        front = (front + 1) % maxQue;
        item = items[front];
        length--;
    }
}

int QueType::Length()
// 4번 문제
{
    if (front < rear)
        return (rear - front);
    else
        return (maxQue + rear - front);
}

bool QueType::Identical(QueType& queue)
// 3번 문제
{
    int j = 0, k = 0;
    for (j, k; (j <= maxQue && k <= maxQue); j++, k++)
    {
        if (items[j] == queue.items[k])
            continue;
        else
            return false;
    }
    return true;
}

void QueType::ReplaceItem(ItemType oldItem, ItemType newItem)
// 2번 문제
{
    for (int i = 0; i <= maxQue; i++) {
        if (items[i] == oldItem)
            items[i] = newItem;
    }
}

int Length(QueType& queue)
// 4번 문제
{
    QueType temp_queue1;
    ItemType item1;
    int count = 0;

    while (!queue.IsEmpty())
    {
        queue.Dequeue(item1);
        count++;
        temp_queue1.Enqueue(item1);
    }
    while (!temp_queue1.IsEmpty())
    {
        temp_queue1.Dequeue(item1);
        queue.Enqueue(item1);
    }

    return count;
}

bool Identical(QueType& queue2, QueType& queue3)
// 3번 문제
{
    QueType temp_queue2;
    QueType temp_queue3;
    ItemType item2;
    ItemType item3;

    while (!queue2.IsEmpty() && !queue3.IsEmpty())
    {
        queue2.Dequeue(item2);
        temp_queue2.Enqueue(item2);
        queue3.Dequeue(item3);
        temp_queue3.Enqueue(item3);
    }
    while (!temp_queue2.IsEmpty() && !temp_queue3.IsEmpty())
    {
        temp_queue2.Dequeue(item2);
        queue2.Enqueue(item2);
        temp_queue3.Dequeue(item3);
        queue3.Enqueue(item3);
        if (item2 == item3)
            continue;
        if (item2 != item3)
            return false;
    }
    return true;
}

void ReplaceItem(QueType& queue, int oldItem, int newItem)
// 2번 문제
{
    QueType temp_queue4;
    ItemType item4;

    while (!queue.IsEmpty())
    {
        queue.Dequeue(item4);
        if (item4 == oldItem)
            temp_queue4.Enqueue(newItem);
        else
            temp_queue4.Enqueue(item4);
    }
    while (!temp_queue4.IsEmpty())
    {
        temp_queue4.Dequeue(item4);
        queue.Enqueue(item4);
    }
}
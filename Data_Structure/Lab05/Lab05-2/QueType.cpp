#include "QueType.h"

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
    minimum_pos = 0;
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
    minimum_pos = 0;
}
QueType::~QueType()         // Class destructor
{
    delete[] items;
}

void QueType::MakeEmpty()
// Post: front and rear have been reset to the empty state.
{
    front = maxQue - 1;
    rear = maxQue - 1;
    minimum_pos = 0;
}

bool QueType::IsEmpty() const
// Returns true if the queue is empty; false otherwise.
{
    return (rear == front);
}

bool QueType::IsFull() const
// Returns true if the queue is full; false otherwise.
{
    return ((rear + 1) % maxQue == front);
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
    }

    minimum_pos = 0;

    for (int i = 0; i <= maxQue; i++)
    {
        if (items[i] == -1)
            items[i] = newItem;
    }

    for (int i = 1; i < maxQue; i++)
    {
        if (items[i] == -1)
            continue;
        if (items[i] < items[minimum_pos])
            minimum_pos = i;
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
    }
}

void QueType::MinDequeue(ItemType& item) // maxQue만큼 들어가는 값을 초기화해야함(최소를 구하는거니까 큰값으로) - 초기화 안되어있어서 쓰레기값이 나오는것
{
    if (IsEmpty())
        throw EmptyQueue();
    else
    {
        minimum_pos = 0;
        for (int i = 1; i < maxQue; i++)
        {
            if (items[i] == -1)
                continue;
            if (items[i] < items[minimum_pos])
                minimum_pos = i;
        }
        item = items[minimum_pos];
        items[minimum_pos] = -1;
    }
}

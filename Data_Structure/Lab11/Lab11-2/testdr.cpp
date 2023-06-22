#include <iostream>
#include "PQType.h"

using namespace std;

int main()
{
	PQType<int> pq;

	pq.Enqueue(12);
	pq.Enqueue(15);
	pq.Enqueue(31);
	pq.Enqueue(7);
	pq.Enqueue(26);

	while (!pq.IsEmpty())
	{
		int item;
		pq.Dequeue(item);
		cout << item << " , " << endl;
	}
}
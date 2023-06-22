#include <iostream>
#include "UnsortedType-1.h"

int main()
{
	int item1 = 1;
	int item2 = 3;
	int item3 = 2;
	int item4 = 5;

	UnsortedType<int> list;

	list.InsertItem(item1);
	list.InsertItem(item2);
	list.InsertItem(item3);
	list.InsertItem(item4);

	list.PrintSumSquares();

	return 0;
}
#include <iostream>
#include "UnsortedType-1.h"

int main()
{
	int item1 = 4;
	int item2 = 1;
	int item3 = 7;
	int item4 = 5;

	UnsortedType<int> list;

	list.InsertItem(item1);
	list.InsertItem(item2);
	list.InsertItem(item3);
	list.InsertItem(item4);

	list.SortPrint(); // listData가 private이기 때문에 SortPrint 함수를 class 내에 구현하여 이용함

	return 0;
}
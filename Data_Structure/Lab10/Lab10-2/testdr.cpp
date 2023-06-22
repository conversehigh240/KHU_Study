#include <iostream>
#include "TreeType.h"
#include "SortedType.h"

using namespace std;

void AddElement(TreeType& tree, int Array[], int from, int to);
void MakeTree(TreeType& tree, SortedType<int>& list);

int main()
{
	int item1 = 4;
	int item2 = 1;
	int item3 = 7;
	int item4 = 6;
	int item5 = 2;
	int item6 = 5;
	int item7 = 3;

	TreeType tree;
	SortedType<int> list;

	list.InsertItem(item1);
	list.InsertItem(item2);
	list.InsertItem(item3);
	list.InsertItem(item4);
	list.InsertItem(item5);
	list.InsertItem(item6);
	list.InsertItem(item7);

	MakeTree(tree, list);
	tree.Print();
}

void MakeTree(TreeType& tree, SortedType<int>& list)
{
	int length = list.LengthIs();
	int* array = new int[length];
	int item_info;
	int i;

	list.ResetList();

	for (i = 0; i < length; i++)
	{
		list.GetNextItem(item_info);
		array[i] = item_info;
	}

	AddElement(tree, array, 0, length - 1);

	delete[] array;
}

void AddElement(TreeType& tree, int Array[], int front, int rear)
{
	int mid;
	if (front <= rear)
	{
		mid = (front + rear) / 2;
		tree.InsertItem(Array[mid]);
		AddElement(tree, Array, front, mid - 1);
		AddElement(tree, Array, mid + 1, rear);
	}
}
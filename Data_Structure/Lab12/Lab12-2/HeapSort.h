#pragma once
#include "Student.h"

template <class ItemType>
void ReheapDown(ItemType values[], int root, int bottom);

template <class ItemType>
void ReheapUp(ItemType values[], int root, int bottom);

template <class ItemType>
int GetHeight(ItemType values[], int start, int numValues);

template <class ItemType>
void Swap(ItemType& item1, ItemType& item2);

template <class ItemType>
void HeapSort(ItemType values[], int numValues)
{
	int index;

	for (index = numValues / 2 - 1; index >= 0; index--)
		ReheapDown(values, index, numValues - 1);

	for (index = numValues - 1; index >= 1; index--)
	{
		Swap(values[0], values[index]);
		ReheapDown(values, 0, index - 1);
	}
}

template <class ItemType>
void ReheapDown(ItemType values[], int root, int bottom)
{
	int maxChild;
	int rightChild;
	int leftChild;

	leftChild = root * 2 + 1;
	rightChild = root * 2 + 2;

	if (leftChild <= bottom)
	{
		if (leftChild == bottom)
			maxChild = leftChild;
		else
		{
			if (values[leftChild] <= values[rightChild])
				maxChild = rightChild;
			else
				maxChild = leftChild;
		}

		if (values[root] < values[maxChild])
		{
			Swap(values[root], values[maxChild]);
			ReheapDown(values, maxChild, bottom);
		}
	}

}

template <class ItemType>
void ReheapUp(ItemType values[], int root, int bottom)
{
	int parent;

	while (bottom > root)
	{
		parent = (bottom - 1) / 2;

		if (values[parent] < values[bottom])
		{
			Swap(values[parent], values[bottom]);
			ReheapUp(values, root, parent);
		}
	}
}

template <class ItemType>
void GetHeightSum(ItemType values[], int numValues)
{
	int index, sum = 0;

	for (index = 0; index < numValues; index++)
		sum += GetHeight(values, index, numValues - 1);

	cout << "sum of heights = " << sum << endl;
}

template <class ItemType>
int GetHeight(ItemType values[], int start, int numValues)
{
	int leftChild = start * 2 + 1;
	int rightChild = start * 2 + 2;

	if (start > numValues / 2)
		return 0;
	int l_height = GetHeight(values, leftChild, numValues);
	int r_height = GetHeight(values, rightChild, numValues);

	if (l_height >= r_height)
		return l_height + 1;
	else
		return r_height + 1;
}

template <class ItemType>
void Swap(ItemType& item1, ItemType& item2)
{
	ItemType temp;

	temp = item1;
	item1 = item2;
	item2 = temp;
}
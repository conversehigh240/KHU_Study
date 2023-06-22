#include "Student.h"

void InsertionSort(Student ary[], int numElems);

void InsertItem(Student values[], int startIndex, int endIndex)
{
	bool finished = false;
	int current = endIndex;
	bool moreToSearch = (current != startIndex);

	while (moreToSearch && !finished)
	{
		if (strcmp(values[current].getName(), values[current - 1].getName()) < 0)
		{
			Swap(values[current], values[current - 1]);
			current--;
			moreToSearch = (current != startIndex);
		}
		else
			finished = true;
	}
}

void InsertionSort(Student ary[], int numElems)
{
	for (int count = 0; count < numElems; count++)
		InsertItem(ary, 0, count);
}
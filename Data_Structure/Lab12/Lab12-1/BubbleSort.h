#include "Student.h"

void BubbleSort(Student ary[], int numElems);

void BubbleUp(Student values[], int startIndex, int endIndex)
{
	for (int index = endIndex; index > startIndex; index--)
	{
		if (strcmp(values[index].getName(), values[index - 1].getName()) < 0)
			Swap(values[index], values[index - 1]);
	}
}

void BubbleSort(Student ary[], int numElems)
{
	int current = 0;

	while (current < numElems - 1)
	{
		BubbleUp(ary, current, numElems - 1);
		current++;
	}
}
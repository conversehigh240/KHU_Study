#pragma once
#include "Student.h"

int MinIndex(Student* values[], int startIndex, int endIndex) {
	// Post: Returns the index of the smallest value in
	//       values[startIndex]..values[endIndex].
	int indexOfMin = startIndex;
	for (int index = startIndex + 1; index <= endIndex; index++)
	{
		if (strcmp(values[index]->getName(), values[indexOfMin]->getName()) < 0)
			indexOfMin = index;
	}
	return indexOfMin;
}

void SelectionSortPointer(Student* values[], int numValues) {

	int endIndex = numValues - 1;
	for (int current = 0; current < endIndex; current++)
		Swap(values[current], values[MinIndex(values, current, endIndex)]);
}
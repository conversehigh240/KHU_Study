#include <iostream>
#include "Student.h"
#include "QuickSort.h"

using namespace std;

int main()
{
	Student stu[100];
	stu[0].InitValue(2003200111, (char*)"이웅재", 3.0);
	stu[1].InitValue(2004200121, (char*)"권오준", 3.2);
	stu[2].InitValue(2005200132, (char*)"김진일", 2.7);

	QuickSort(stu, 0, 2);
	Print(cout, stu, 3);

	return 0;
}
#include <iostream>
#include "Student.h"
#include "HeapSort.h"

using namespace std;

int main()
{
	cout << "----------Q4----------" << endl;
	Student stu1[100];
	stu1[0].InitValue(2003200111, (char*)"ÀÌ¿õÀç", 3.0);
	stu1[1].InitValue(2004200121, (char*)"±Ç¿ÀÁØ", 3.2);
	stu1[2].InitValue(2005200132, (char*)"±èÁøÀÏ", 2.7);

	HeapSort(stu1, 3);
	Print(cout, stu1, 3);
	cout << endl;

	cout << "----------Q6----------" << endl;
	int value[100] = { 25, 17, 36, 2, 3, 100, 1, 19, 7 };

	HeapSort(value, 9);
	GetHeightSum(value, 9);
	cout << endl;
}
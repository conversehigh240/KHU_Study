#include <iostream>
#include "Student.h"
#include "SelectionSort.h"
#include "BubbleSort.h"
#include "InsertionSort.h"


using namespace std;

int main()
{
	cout << "----------Q1----------" << endl;
	Student stu1[100];
	stu1[0].InitValue(2003200111, (char*)"ÀÌ¿õÀç", 3.0);
	stu1[1].InitValue(2004200121, (char*)"±Ç¿ÀÁØ", 3.2);
	stu1[2].InitValue(2005200132, (char*)"±èÁøÀÏ", 2.7);

	SelectionSort(stu1, 3);
	Print(cout, stu1, 3);
	cout << endl;

	cout << "----------Q2----------" << endl;
	Student stu2[100];
	stu2[0].InitValue(2003200111, (char*)"ÀÌ¿õÀç", 3.0);
	stu2[1].InitValue(2004200121, (char*)"±Ç¿ÀÁØ", 3.2);
	stu2[2].InitValue(2005200132, (char*)"±èÁøÀÏ", 2.7);

	BubbleSort(stu2, 3);
	Print(cout, stu2, 3);
	cout << endl;

	cout << "----------Q3----------" << endl;
	Student stu3[100];
	stu3[0].InitValue(2003200111, (char*)"ÀÌ¿õÀç", 3.0);
	stu3[1].InitValue(2004200121, (char*)"±Ç¿ÀÁØ", 3.2);
	stu3[2].InitValue(2005200132, (char*)"±èÁøÀÏ", 2.7);

	InsertionSort(stu3, 3);
	Print(cout, stu3, 3);

	return 0;
}
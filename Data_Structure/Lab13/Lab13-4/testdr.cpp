#include <iostream>
#include "Student.h"
#include "HashTable.h"
using namespace std;

int main() {
	Student stu[100];
	stu[0].InitValue(2003200111, (char*)"Lee", 3.0);
	stu[1].InitValue(2004200121, (char*)"Kwon", 3.2);
	stu[2].InitValue(2005200132, (char*)"Kim", 2.7);
	stu[3].InitValue(2006200142, (char*)"Jang", 2.7);

	Student emp;
	emp.InitValue(0000000000, (char*)"None", 0.0);

	HashTable<Student> ht(emp);

	for (int i = 0; i < 3; i++)
		ht.InsertItem(stu[i]); 



	bool found = false;
	ht.RetrieveItem(stu[3], found);
	cout << found << endl;

	ht.RetrieveItem(stu[0], found); 
	cout << found << endl;
	cout << stu[0].getName() << endl;

	return 0;
}
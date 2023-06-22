#include <iostream>
#include "SortedType.h"

using namespace std;

int main() {

	SortedType<char> list;

	list.InsertItem('A');
	list.InsertItem('B');
	list.InsertItem('C');
	list.InsertItem('D');
	list.InsertItem('E');

	list.PrintReverse();
}
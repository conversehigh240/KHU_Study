// Test driver
#include <iostream>

#include "StackType.h"

using namespace std;

int main()
{
	StackType stack;
	StackType stack_copy;

	int elem[5] = { 7, 4, 5, 6, 10 };
	for (int i : elem)
		stack.Push(i);

	stack.Copy(stack_copy);

	while (!stack_copy.IsEmpty())
	{
		cout << stack_copy.Top() << endl;
		stack_copy.Pop();
	}
}

#include <iostream>
#include "StackType.h"

using namespace std;

int main()
{
	StackType stack;

	stack.Push(12);
	stack.Push(15);
	stack.Push(31);
	stack.Push(7);
	stack.Push(26);

	while (!stack.IsEmpty())
	{
		cout << stack.Top() << endl;
		stack.Pop();
	}
}
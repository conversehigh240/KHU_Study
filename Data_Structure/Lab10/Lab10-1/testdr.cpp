#include <iostream>
#include "TreeType.h"

using namespace std;
int Smaller(TreeType tree, int value);

int main()
{
	int item1 = 26;
	int item2 = 12;
	int item3 = 3;
	int item4 = 15;
	int item5 = 7;
	int item6 = 38;
	int item7 = 21;

	TreeType tree;

	tree.InsertItem(item1);
	tree.InsertItem(item2);
	tree.InsertItem(item3);
	tree.InsertItem(item4);
	tree.InsertItem(item5);
	tree.InsertItem(item6);
	tree.InsertItem(item7);

	tree.Print();
	tree.Ancestors_n(21);
	tree.Ancestors_re(21);
	cout << Smaller(tree, 15) << endl;

}

int Smaller(TreeType tree, int value)
{
	ItemType item;
	bool finished = false;
	int count = 0;

	tree.ResetTree(IN_ORDER);

	do
	{
		tree.GetNextItem(item, IN_ORDER, finished);
		if (item < value)
			count++;
		else
			finished = true;
	} while (!finished);

	return count;
}
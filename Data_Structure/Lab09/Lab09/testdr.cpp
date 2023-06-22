#include <iostream>
#include "TreeType.h"

using namespace std;

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

	cout << tree.IsBST() << endl;
	cout << tree.LeafCount() << endl;
	cout << tree.SingleParentCount() << endl;
}
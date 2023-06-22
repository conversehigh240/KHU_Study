#include <iostream>
#include "TreeType.h"
#include "UnsortedType.h"

using namespace std;

bool MatchingItem_Unsorted(TreeType& tree, UnsortedType<ItemType>& list);

int main()
{
	int item1 = 4;
	int item2 = 1;
	int item3 = 7;
	int item4 = 6;
	int item5 = 2;
	int item6 = 5;
	int item7 = 3;

	TreeType tree;
	UnsortedType<int> list;

	tree.InsertItem(item1);
	tree.InsertItem(item2);
	tree.InsertItem(item3);
	tree.InsertItem(item4);
	tree.InsertItem(item5);
	tree.InsertItem(item6);
	tree.InsertItem(item7);

	list.InsertItem(item2);
	list.InsertItem(item1);
	list.InsertItem(item4);
	list.InsertItem(item6);
	list.InsertItem(item7);
	list.InsertItem(item3);
	list.InsertItem(item5);

	cout << MatchingItem_Unsorted(tree, list) << endl;
}

bool MatchingItem_Unsorted(TreeType& tree, UnsortedType<ItemType>& list)
{
	int list_length = list.LengthIs();
	int tree_length = tree.LengthIs();

	if (list_length != tree_length)
	{
		return false;
	}
	else
	{
		ItemType item;
		bool found;

		list.ResetList();
		for (int i = 0; i < list_length; i++)
		{
			list.GetNextItem(item);
			tree.RetrieveItem(item, found);
			if (!found)
				return false;
		}
		return true;
	}
}
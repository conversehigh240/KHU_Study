
#pragma once
#include "Student.h"

const int MAX_ITEMS = 20000;

template <class ItemType>
class HashTable {
	ItemType info[MAX_ITEMS];
	ItemType emptyItem;
	int length;
public:
	HashTable() {}
	HashTable(ItemType emptyKey);
	int Hash(char* key) const;
	void RetrieveItem(ItemType& item, bool& found);
	void InsertItem(ItemType item);
};

int getIntFromString(char* key)
{
	int sum = 0;
	int len = strlen(key);
	if (len % 2 == 1) len++; // make len even
	for (int j = 0; j < len; j += 2)
		sum = (sum + 100 * key[j] + key[j + 1]) % 19937;
	return sum;
}

template <class ItemType>
HashTable<ItemType>::HashTable(ItemType emptyItem) {
	this->emptyItem = emptyItem;
	length = 0;
	for (int i = 0; i < MAX_ITEMS; i++)
		info[i] = emptyItem;
}

template <class ItemType>
int HashTable<ItemType>::Hash(char* key) const
{
	return (getIntFromString(key) % MAX_ITEMS);
}

template<class ItemType>
void HashTable<ItemType>::InsertItem(ItemType item)
{
	int location;
	location = Hash(item.Key());
	while (info[location] != emptyItem)
		location = (location + 1) % MAX_ITEMS;
	info[location] = item;
	length++;
}

template<class ItemType>
void HashTable<ItemType>::RetrieveItem(ItemType& item, bool& found) {
	int location;
	int startLoc;
	bool moreToSearch = true;

	startLoc = Hash(item.Key());
	location = startLoc;
	do {
		if (info[location] == item || info[location] == emptyItem)
			moreToSearch = false;
		else
			location++;
	} while (moreToSearch);
	found = (info[location] == item);
	if (found)
		item = info[location];
}
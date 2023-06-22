#pragma once
const int MAX_ITEMS = 200;

class DoubleStack
{
private:
	int top_small;
	int top_big;
	int items[MAX_ITEMS];

public:
	DoubleStack();
	bool IsEmpty() const;
	bool IsFull() const;
	void Push(int item);
	void Pop();
	void Print();
};

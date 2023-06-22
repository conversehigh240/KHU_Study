#include "DoubleStack.h"
#include <iostream>

using namespace std;

DoubleStack::DoubleStack() {
	top_small = -1;
	top_big = 199;
}
void DoubleStack::Push(int item) {
	if (item <= 1000) {
		top_small++;
		items[top_small] = item;
	}
	if (item > 1000) {
		top_big++;
		items[top_big] = item;
	}
	if (IsFull())
		return;
}

bool DoubleStack::IsEmpty() const {
	return (top_small == -1 && top_big == 199);
}

bool DoubleStack::IsFull() const {
	return (top_small == top_big - 1);
}

void DoubleStack::Pop() {
	if (IsEmpty())
		return;
	top_small--;
	top_big--;
}

void DoubleStack::Print() {
	for (int i = top_big; i <= 199; i--) {
		for (int j = top_small; i <= -1; i--) {
			cout << items[j] << endl;
		}
		cout << items[i] << endl;
	}
}
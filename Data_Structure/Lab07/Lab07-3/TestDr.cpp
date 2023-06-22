#include <iostream>

#include "TextEditor.h"

using namespace std;

int main() {

	TextEditor text;
	ItemType text1[80] = "This is the first line of text";
	ItemType text2[80] = "This is the current line of text";
	ItemType text3[80] = "This is the last line of text";

	text.InsertItem(text1);
	text.InsertItem(text2);
	text.InsertItem(text3);

	text.Print();
}
#include <iostream>

using namespace std;

// Binary Search (이진탐색) - A 문제
int BinarySearch(int a[], int sizeOfArray, int value) {
	int midPoint;
	int first = 0;
	int last = sizeOfArray - 1;

	while (last - first >= 0) {
		midPoint = (first + last) / 2;
		if (a[midPoint] == value)
			return midPoint;
		else if (a[midPoint] > value)
			last = midPoint - 1;
		else
			first = midPoint + 1;
	}
	return -1;
}

//Binary Search (이진탐색) - B 문제
/*int BinarySearch(int a[], int sizeOfArray, int value) {
	int midPoint;
	int first = 0;
	int last = sizeOfArray - 1;

	while (last - first >= 0) {
		midPoint = (first + last) / 2;
		if (a[midPoint] <= value)
			first = midPoint + 1;
		else
			last = midPoint - 1;
	}
	return a[last];
}*/

//Binary Search (이진탐색) - C 문제
/*int BinarySearch(int a[], int sizeOfArray, int value) {
	int midPoint;
	int first = 0;
	int last = sizeOfArray - 1;

	while (last - first > 0) {
		midPoint = (first + last) / 2;
		if (a[midPoint] >= value)
			last = midPoint;
		else
			first = midPoint + 1;
	}
	return a[last];
}*/

int main()
{
	int list[10] = { 1,2,3,4,5,6,7,8,9,10 };
	int result = BinarySearch(list, 10, 11);
	cout << result << endl; // -1 리턴
	result = BinarySearch(list, 10, 7);
	cout << result << endl; // 6 리턴
	return 0;
}


#include <iostream>

using namespace std;

int NumPaths(int row, int col, int n);
int NumPaths_C(int row, int col, int n);

const int MAX_ROWS = 10;
const int MAX_COLS = 10;
int mat[MAX_ROWS][MAX_COLS];

int main() {

	for (int i = 0; i < MAX_ROWS; i++)
		for (int j = 0; j < MAX_COLS; j++)
			mat[i][j] = -1;

	cout << NumPaths(1, 1, 5) << endl;
	cout << NumPaths(1, 1, 5) << endl;

}

int NumPaths(int row, int col, int n)
{
	if ((row == n) || (col == n))
		return 1;
	else
		return NumPaths(row + 1, col, n) + NumPaths(row, col + 1, n);

}

int NumPaths_C(int row, int col, int n)
{
	if (mat[row][col] == -1)
	{
		if ((row == n) || (col == n))
			mat[row][col] = 1;
		else
			mat[row][col] = NumPaths_C(row + 1, col, n) + NumPaths_C(row, col + 1, n);
	}
	return mat[row][col];
}
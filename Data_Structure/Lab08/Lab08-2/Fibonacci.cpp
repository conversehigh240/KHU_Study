/*재귀 버전이 더 효율적이지 못하다.
재귀 버전이 효율적이지 못한 경우 중 하나가 기본형으로 리턴을 먼저 만든 뒤
그것을 제외하고 점화식의 형태로 진행되는 내용일 경우이다.
이런 경우 중 fibonacci의 경우 fibonacci(n)을 구하기 위해 fibonacci(n-1)과 fibonacci(n-2)를 구해야 하는데
이런 경우 중복계산이 여러번 진행되기 때문에 더 효율적이지 못하다.
또, 재귀호출이라고 하면 활성화 레코드 쪽에 스택에 하나하나 값이 쌓여있게 되는데, 이런 경우 중복항들이 여러개 생기게 된다.
하지만 비재귀호출의 경우 값을 그냥 참조해오면 되기 때문에 이런 문제가 발생하지 않늗다.*/

#include <iostream>

using namespace std;

int Fibonacci_recursive(int n);
int Fibonacci_non_recursive(int n);

int main()
{
	cout << Fibonacci_recursive(10) << endl;
	cout << Fibonacci_non_recursive(10) << endl;

	return 0;
}

int Fibonacci_recursive(int n)
{
	if (n == 0)
		return 0;
	else if (n == 1)
		return 1;
	else
		return Fibonacci_recursive(n - 1) + Fibonacci_recursive(n - 2);
}

int Fibonacci_non_recursive(int n)
{
	int a = 0, b = 1, c;
	if (n == 0)
		return 0;
	else if (n == 1)
		return 1;
	else
	{
		for (int i = 2; i <= n; i++)
		{
			c = a + b;
			a = b;
			b = c;
		}
		return c;
	}
}
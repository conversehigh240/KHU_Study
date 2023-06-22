/*��� ������ �� ȿ�������� ���ϴ�.
��� ������ ȿ�������� ���� ��� �� �ϳ��� �⺻������ ������ ���� ���� ��
�װ��� �����ϰ� ��ȭ���� ���·� ����Ǵ� ������ ����̴�.
�̷� ��� �� fibonacci�� ��� fibonacci(n)�� ���ϱ� ���� fibonacci(n-1)�� fibonacci(n-2)�� ���ؾ� �ϴµ�
�̷� ��� �ߺ������ ������ ����Ǳ� ������ �� ȿ�������� ���ϴ�.
��, ���ȣ���̶�� �ϸ� Ȱ��ȭ ���ڵ� �ʿ� ���ÿ� �ϳ��ϳ� ���� �׿��ְ� �Ǵµ�, �̷� ��� �ߺ��׵��� ������ ����� �ȴ�.
������ �����ȣ���� ��� ���� �׳� �����ؿ��� �Ǳ� ������ �̷� ������ �߻����� �ʈf��.*/

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
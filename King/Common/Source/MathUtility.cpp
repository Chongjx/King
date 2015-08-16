#include "MathUtility.h"

int factorial(int n)
{
	if (n == 0 || n == 1)
	{
		return 1;
	}

	else if (n < 0)
	{
		return 0;
	}
	
	return n * factorial(n - 1);
}

int nCr(int n, int r)
{
	if (n >= r && n >= 0 && r >= 0)
	{
		return factorial(n)/(factorial(n - r) * factorial(r));
	}

	return 0;
}

int nPr(int n, int r)
{
	if (n >= r)
	{
		return factorial(n)/factorial(n - r);
	}

	return 0;
}

int nTermAP(int a, int d, int n)
{
	if (n >= 1)
	{
		return (a + (n-1) * d);
	}

	return 0;
}

int summationAP(int a, int d, int n)
{
	if (n >= 1)
	{
		return (n * (a + nTermAP(a, d, n))/2);
	}
	return 0;
}

int nTermGP(int a, int r, int n)
{
	if (n >= 1)
	{
		return a * pow(static_cast<double>(r), static_cast<double>(n - 1));
	}

	return 0;
}

int summationGP(int a, int r, int n)
{
	if (n >= 1 && r > 1)
	{
		return a * (( 1 - pow(static_cast<double>(r), static_cast<double>(n)))/ (1 - r));
	}
	return 0;
}
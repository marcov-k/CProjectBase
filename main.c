#include "include/m_string.h"
#include <stdio.h>

int main()
{
	String s = CreateString("Hello World!");

	printf("Original string: ");
	PrintStringLn(&s);

	ReverseString(&s);

	printf("\nReversed string: ");
	PrintStringLn(&s);

	DestroyString(&s);

	return 0;
}
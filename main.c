#include "include/m_string.h"
#include <stdio.h>

int main()
{
	String s = CreateString("Hello World!");
	printf("Original string: ");
	PrintStringLn(&s);

	String s2 = CopyString(&s);
	printf("Copied string: ");
	PrintStringLn(&s2);

	size_t start = 5, length = 6;
	RemoveStringRange(&s2, start, length);
	printf("\nRange from index %zu to %zu removed: ", start, start + length - 1);
	PrintStringLn(&s2);

	ReverseString(&s);
	printf("\nReversed string: ");
	PrintStringLn(&s);

	DestroyString(&s);
	DestroyString(&s2);

	return 0;
}
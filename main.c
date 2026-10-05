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

	RemoveCharAll(&s2, 'l');
	printf("\nRemoved every \'l\': ");
	PrintStringLn(&s2);

	ReverseString(&s);
	printf("\nReversed string: ");
	PrintStringLn(&s);

	DestroyString(&s);
	DestroyString(&s2);

	return 0;
}
#include "include/m_string.h"
#include "include/m_vector.h"

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

	int nums[] = { 1, 2, 3, 4, 5 };
	Vector v = CreateVector(nums, 5, sizeof(int), NULL);
	printf("\nNumber vector created: ");
	for (size_t i = 0; i < v.length; ++i)
	{
		int value = deref(GetElementAt(&v, i), int);
		printf("%d, ", value);
	}

	DestroyVector(&v);

	return 0;
}
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

	StringRemoveCharAll(&s2, 'l');
	printf("\nRemoved every \'l\': ");
	PrintStringLn(&s2);

	ReverseString(&s);
	printf("\nReversed original string: ");
	PrintStringLn(&s);

	DestroyString(&s);
	DestroyString(&s2);

	int nums[] = { 1, 2, 3, 4, 5 };
	Vector v = CreateVector(nums, 5, sizeof(int), NULL, NULL);
	printf("\nNumber vector created: ");
	for (size_t i = 0; i < v.length; ++i)
	{
		int value = deref(VectorGetElementAt(&v, i), int);
		printf("%d, ", value);
	}

	Vector v2 = CopyVector(&v, NULL);
	printf("\nCopied vector: ");
	for (size_t i = 0; i < v2.length; ++i)
	{
		int value = deref(VectorGetElementAt(&v2, i), int);
		printf("%d, ", value);
	}

	int newVal = 10;
	size_t changeIndex = 1;
	VectorSetElementAt(&v2, changeIndex, &newVal, NULL);
	printf("\n\nChanged element at index %zu to %d: ", changeIndex, newVal);
	for (size_t i = 0; i < v2.length; ++i)
	{
		int value = deref(VectorGetElementAt(&v2, i), int);
		printf("%d, ", value);
	}

	newVal = 150;
	VectorPushBack(&v, &newVal, NULL);
	printf("\n\nAppended %d to original vector: ", newVal);
	for (size_t i = 0; i < v.length; ++i)
	{
		int value = deref(VectorGetElementAt(&v, i), int);
		printf("%d, ", value);
	}

	printf("\n\nCopied vector: ");
	for (size_t i = 0; i < v2.length; ++i)
	{
		int value = deref(VectorGetElementAt(&v2, i), int);
		printf("%d, ", value);
	}

	printf("\nLast element in vector: %d", deref(VectorGetElementLast(&v2), int));

	VectorPopBack(&v2);
	printf("\n\nRemoved last element: ");
	for (size_t i = 0; i < v2.length; ++i)
	{
		int value = deref(VectorGetElementAt(&v2, i), int);
		printf("%d, ", value);
	}

	DestroyVector(&v);
	DestroyVector(&v2);

	printf("\n");
	return 0;
}
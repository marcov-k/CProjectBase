#include "include/m_string.h"
#include <stdio.h>

int main()
{
	String s = CreateString("AB");
	size_t splitIndex = 1;

	printf("Original string: ");
	PrintStringLn(&s);
	printf("\nSplitting at index %zu...\n\n", splitIndex);

	StringSplit split = SplitString(&s, splitIndex);

	if (split.string1.data != NULL)
	{
		printf("Contents of string 1: ");
		PrintStringLn(&split.string1);
	}

	if (split.string2.data != NULL)
	{
		printf("Contents of string 2: ");
		PrintStringLn(&split.string2);
	}

	DestroyString(&s);
	DestroyStringSplit(&split);

	s = CreateString("Hello World");
	size_t trim = 2;

	printf("\n\nOriginal string: ");
	PrintStringLn(&s);
	
	printf("\nRemoving %zu characters from start...\n", trim);

	TrimStringStart(&s, trim);
	
	printf("Trimmed string: ");
	PrintStringLn(&s);

	trim = 4;
	printf("\nRemoving %zu characters from end...\n", trim);

	TrimStringEnd(&s, trim);

	printf("Trimmed string: ");
	PrintStringLn(&s);

	DestroyString(&s);

	return 0;
}
#include "include/m_string.h"
#include <stdio.h>

int main()
{
	String hello = CreateString("Hello ");
	String world = CreateString("World");
	String s = ConcatStrings(&hello, &world);

	printf("%s", s.data);

	DestroyString(&hello);
	DestroyString(&world);
	DestroyString(&s);
	return 0;
}
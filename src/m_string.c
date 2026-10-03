#include "../include/m_string.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

String CreateString(const char s[])
{
	String string = { NULL, 0 };

	if (s == NULL) return string;

	string.length = strlen(s);
	string.data = malloc(string.length + 1);
	memcpy(string.data, s, string.length + 1);

	return string;
}

void DestroyString(String* s)
{
	if (s == NULL) return;

	if (s->data != NULL) // only free memory if allocated
	{
		free(s->data);
		s->data = NULL;
	}
	s->length = 0;
}

String CopyString(const String* s)
{
	String copy = { NULL, 0 };

	if (s == NULL || s->data == NULL) return copy;

	copy.length = s->length;
	copy.data = malloc(s->length + 1);
	memcpy(copy.data, s->data, s->length + 1);

	return copy;
}

void CopyStringTo(String* dest, const String* source)
{
	if (dest == NULL || source == NULL || source->data == NULL) return;

	if (dest->length != source->length)
	{
		DestroyString(dest);
		dest->length = source->length;
		dest->data = malloc(source->length + 1);
	}

	memcpy(dest->data, source->data, source->length + 1);
}

void CopyCStrToString(String* dest, const char source[])
{
	if (dest == NULL || source == NULL) return;

	if (dest->length != strlen(source))
	{
		DestroyString(dest);
		dest->length = strlen(source);
		dest->data = malloc(dest->length + 1);
	}

	memcpy(dest->data, source, dest->length + 1);
}

void DestroyStringSplit(StringSplit* split)
{
	if (split == NULL) return;

	DestroyString(&split->string1);
	DestroyString(&split->string2);
}

void PrintString(const String* s)
{
	printf("%s", s->data);
}

void PrintStringLn(const String* s)
{
	printf("%s\n", s->data);
}

String ConcatStrings(const String* s1, const String* s2)
{
	String result = { NULL, 0 };

	if (s1 != NULL && s1->data != NULL && s2 != NULL && s2->data != NULL)
	{
		result.length = s1->length + s2->length;
		result.data = malloc(result.length + 1);
		memcpy(result.data, s1->data, s1->length);
		memcpy(result.data + s1->length, s2->data, s2->length + 1);
	}
	else if (s1 != NULL && s1->data != NULL)
	{
		result.length = s1->length;
		result.data = malloc(s1->length + 1);
		memcpy(result.data, s1->data, s1->length + 1);
	}
	else if (s2 != NULL && s2->data != NULL)
	{
		result.length = s2->length;
		result.data = malloc(s2->length + 1);
		memcpy(result.data, s2->data, s2->length + 1);
	}

	return result;
}

String ConcatStringCStr(const String* s1, const char s2[])
{
	String result = { NULL, 0 };

	if (s1 != NULL && s1->data != NULL && s2 != NULL)
	{
		size_t s2Len = strlen(s2);

		result.length = s1->length + s2Len;
		result.data = malloc(result.length + 1);
		memcpy(result.data, s1->data, s1->length);
		memcpy(result.data + s1->length, s2, s2Len);
	}
	else if (s1 != NULL && s1->data != NULL)
	{
		result.length = s1->length;
		result.data = malloc(s1->length + 1);
		memcpy(result.data, s1->data, s1->length + 1);
	}
	else if (s2 != NULL)
	{
		result.length = strlen(s2);
		result.data = malloc(result.length + 1);
		memcpy(result.data, s2, result.length);
	}

	return result;
}

String ConcatCStrString(const char s1[], const String* s2)
{
	String result = { NULL, 0 };

	if (s1 != NULL && s2 != NULL && s2->data != NULL)
	{
		size_t s1Len = strlen(s1);

		result.length = s1Len + s2->length;
		result.data = malloc(result.length + 1);
		memcpy(result.data, s1, s1Len);
		memcpy(result.data + s1Len, s2->data, s2->length + 1);
	}
	else if (s1 != NULL)
	{
		result.length = strlen(s1);
		result.data = malloc(result.length + 1);
		memcpy(result.data, s1, result.length + 1);
	}
	else if (s2 != NULL && s2->data != NULL)
	{
		result.length = s2->length;
		result.data = malloc(s2->length + 1);
		memcpy(result.data, s2->data, s2->length);
	}

	return result;
}

void ReverseString(String* s)
{
	if (s == NULL || s->data == NULL) return;

	char* reverse = malloc(s->length + 1);

	for (size_t i = 0; i < s->length; ++i)
	{
		reverse[i] = s->data[s->length - i - 1];
	}
	reverse[s->length] = '\0';

	CopyCStrToString(s, reverse);
	free(reverse);
}

StringSplit SplitString(const String* s, size_t index)
{
	StringSplit split = { { NULL, 0 }, { NULL, 0 } };

	if (s == NULL || s->data == NULL) return split;

	if (index >= s->length)
	{
		split.string1.length = s->length;
		split.string1.data = malloc(s->length + 1);
		memcpy(split.string1.data, s->data, s->length + 1);
	}
	else if (index == 0)
	{
		split.string2.length = s->length;
		split.string2.data = malloc(s->length + 1);
		memcpy(split.string2.data, s->data, s->length + 1);
	}
	else
	{
		split.string1.length = index;
		split.string1.data = malloc(index + 1);
		memcpy(split.string1.data, s->data, index);
		split.string1.data[index] = '\0';

		split.string2.length = s->length - index;
		split.string2.data = malloc(split.string2.length + 1);
		memcpy(split.string2.data, s->data + index, split.string2.length + 1);
		split.string2.data[split.string2.length] = '\0';
	}

	return split;
}

void TrimStringStart(String* s, size_t count)
{
	if (s == NULL || s->data == NULL || count == 0) return;

	if (count >= s->length)
	{
		DestroyString(s);
		return;
	}

	size_t length = s->length - count;
	char* temp = malloc(length);
	memcpy(temp, s->data + count, length);
	DestroyString(s);

	s->length = length;
	s->data = malloc(length + 1);
	memcpy(s->data, temp, length);
	s->data[length] = '\0';

	free(temp);
}

void TrimStringEnd(String* s, size_t count)
{
	if (s == NULL || s->data == NULL || count == 0) return;

	if (count >= s->length)
	{
		DestroyString(s);
		return;
	}

	size_t length = s->length - count;
	char* temp = malloc(length);
	memcpy(temp, s->data, length);
	DestroyString(s);

	s->length = length;
	s->data = malloc(length + 1);
	memcpy(s->data, temp, length);
	s->data[length] = '\0';

	free(temp);
}
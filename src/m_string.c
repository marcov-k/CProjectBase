#include "../include/m_string.h"
#include "../include/math_utils.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

String CreateString(const char s[])
{
	String string = { NULL, 0, 0 };

	if (s == NULL) return string;

	size_t length = strlen(s);

	string.data = malloc(length + 1);
	if (string.data == NULL) return string;

	string.capacity = length + 1;
	string.length = length;
	memcpy(string.data, s, length + 1);

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
	s->capacity = 0;
	s->length = 0;
}

String CopyString(const String* s)
{
	String copy = { NULL, 0, 0 };

	if (s == NULL || s->data == NULL) return copy;

	copy.data = malloc(s->length + 1);
	if (copy.data == NULL) return copy;

	copy.capacity = s->length + 1;
	copy.length = s->length;
	memcpy(copy.data, s->data, s->length + 1);

	return copy;
}

void CopyStringTo(String* dest, const String* source)
{
	if (dest == source || dest == NULL || source == NULL || source->data == NULL) return;

	if (dest->capacity < source->length)
	{
		DestroyString(dest);
		dest->data = malloc(source->length + 1);
		if (dest->data == NULL) return;

		dest->capacity = source->length + 1;
		dest->length = source->length;
	}

	memcpy(dest->data, source->data, source->length + 1);
}

void CopyCStrToString(String* dest, const char source[])
{
	if (dest == NULL || source == NULL) return;

	size_t length = strlen(source);
	if (dest->capacity < length)
	{
		DestroyString(dest);
		dest->data = malloc(length + 1);
		if (dest->data == NULL) return;

		dest->capacity = length + 1;
		dest->length = length;
	}

	memcpy(dest->data, source, length + 1);
}

void DestroyStringSplit(StringSplit* split)
{
	if (split == NULL) return;

	DestroyString(&split->string1);
	DestroyString(&split->string2);
}

void PrintString(const String* s)
{
	if (s == NULL || s->data == NULL) return;

	printf("%s", s->data);
}

void PrintStringLn(const String* s)
{
	if (s == NULL || s->data == NULL) return;

	printf("%s\n", s->data);
}

String ConcatStrings(const String* s1, const String* s2)
{
	String result = { NULL, 0, 0 };

	if (s1 != NULL && s1->data != NULL && s2 != NULL && s2->data != NULL)
	{
		size_t length = s1->length + s2->length;
		result.data = malloc(length + 1);
		if (result.data == NULL) return result;

		result.capacity = length + 1;
		result.length = length;
		memcpy(result.data, s1->data, s1->length);
		memcpy(result.data + s1->length, s2->data, s2->length + 1);
	}
	else if (s1 != NULL && s1->data != NULL)
	{
		result.data = malloc(s1->length + 1);
		if (result.data == NULL) return result;

		result.capacity = s1->length + 1;
		result.length = s1->length;
		memcpy(result.data, s1->data, result.capacity);
	}
	else if (s2 != NULL && s2->data != NULL)
	{
		result.data = malloc(s2->length + 1);
		if (result.data == NULL) return result;

		result.capacity = s2->length + 1;
		result.length = s2->length;
		memcpy(result.data, s2->data, result.capacity);
	}

	return result;
}

String ConcatStringCStr(const String* s1, const char s2[])
{
	String result = { NULL, 0, 0 };

	if (s1 != NULL && s1->data != NULL && s2 != NULL)
	{
		size_t s2Len = strlen(s2);
		size_t length = s1->length + s2Len;

		result.data = malloc(length + 1);
		if (result.data == NULL) return result;

		result.capacity = length + 1;
		result.length = length;
		memcpy(result.data, s1->data, s1->length);
		memcpy(result.data + s1->length, s2, s2Len + 1);
	}
	else if (s1 != NULL && s1->data != NULL)
	{
		result.data = malloc(s1->length + 1);
		if (result.data == NULL) return result;

		result.capacity = s1->length + 1;
		result.length = s1->length;
		memcpy(result.data, s1->data, result.capacity);
	}
	else if (s2 != NULL)
	{
		size_t length = strlen(s2);
		result.data = malloc(length + 1);
		if (result.data == NULL) return result;

		result.capacity = length + 1;
		result.length = length;
		memcpy(result.data, s2, result.capacity);
	}

	return result;
}

String ConcatCStrString(const char s1[], const String* s2)
{
	String result = { NULL, 0, 0 };

	if (s1 != NULL && s2 != NULL && s2->data != NULL)
	{
		size_t s1Len = strlen(s1);
		size_t length = s1Len + s2->length;

		result.data = malloc(length + 1);
		if (result.data == NULL) return result;

		result.capacity = length + 1;
		result.length = length;
		memcpy(result.data, s1, s1Len);
		memcpy(result.data + s1Len, s2->data, s2->length + 1);
	}
	else if (s1 != NULL)
	{
		size_t s1Len = strlen(s1);
		result.data = malloc(s1Len + 1);
		if (result.data == NULL) return result;

		result.capacity = s1Len + 1;
		result.length = s1Len;
		memcpy(result.data, s1, result.capacity);
	}
	else if (s2 != NULL && s2->data != NULL)
	{
		result.data = malloc(s2->length + 1);
		if (result.data == NULL) return result;

		result.capacity = s2->length + 1;
		result.length = s2->length;
		memcpy(result.data, s2->data, result.capacity);
	}

	return result;
}

void ReverseString(String* s)
{
	if (s == NULL || s->data == NULL) return;

	size_t halfLen = s->length / 2;
	for (size_t i = 0; i < halfLen; ++i)
	{
		size_t otherIndex = s->length - i - 1;
		char temp = s->data[i];
		s->data[i] = s->data[otherIndex];
		s->data[otherIndex] = temp;
	}
	s->data[s->length] = '\0';
}

StringSplit SplitString(const String* s, size_t index)
{
	StringSplit split = { { NULL, 0 }, { NULL, 0 } };

	if (s == NULL || s->data == NULL) return split;

	if (index >= s->length)
	{
		split.string1.data = malloc(s->length + 1);
		if (split.string1.data == NULL) return split;

		split.string1.capacity = s->length + 1;
		split.string1.length = s->length;
		memcpy(split.string1.data, s->data, split.string1.capacity);
	}
	else if (index == 0)
	{
		split.string2.data = malloc(s->length + 1);
		if (split.string2.data == NULL) return split;

		split.string2.capacity = s->length + 1;
		split.string2.capacity = s->length;
		memcpy(split.string2.data, s->data, split.string2.capacity);
	}
	else
	{
		split.string1.data = malloc(index + 1);
		if (split.string1.data == NULL) return split;

		split.string2.data = malloc(s->length - index + 1);
		if (split.string2.data == NULL)
		{
			DestroyString(&split.string1);
			return split;
		}

		split.string1.capacity = index + 1;
		split.string1.length = index;
		memcpy(split.string1.data, s->data, index);
		split.string1.data[index] = '\0';

		split.string2.capacity = s->length - index + 1;
		split.string2.length = s->length - index;
		memcpy(split.string2.data, s->data + index, split.string2.capacity);
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

	s->length -= count;
	memmove(s->data, s->data + count, s->length);
	s->data[s->length] = '\0';
}

void TrimStringEnd(String* s, size_t count)
{
	if (s == NULL || s->data == NULL || count == 0) return;

	if (count >= s->length)
	{
		DestroyString(s);
		return;
	}

	s->length -= count;
	s->data[s->length] = '\0';
}

void TrimString(String* s, size_t start, size_t end)
{
	if (s == NULL || s->data == NULL) return;

	if (start >= end) return;

	if (start == 0 && end >= s->length) return;

	if (start >= s->length)
	{
		DestroyString(s);
		return;
	}

	start = CLAMP(start, 0, s->length - 1);
	end = CLAMP(end, 0, s->length);

	s->length = end - start;
	memmove(s->data, s->data + start, s->length);
	s->data[s->length] = '\0';
}

String ExtractSubstring(const String* s, size_t start, size_t end)
{
	String substring = { NULL, 0, 0 };

	if (s == NULL || s->data == NULL || start >= s->length) return substring;

	if (start >= end) return substring;

	if (start == 0 && end >= s->length)
	{
		CopyStringTo(&substring, s);
		return substring;
	}

	start = CLAMP(start, 0, s->length - 1);
	end = CLAMP(end, 0, s->length);

	size_t length = end - start;
	substring.data = malloc(length + 1);
	if (substring.data == NULL) return substring;

	substring.capacity = length + 1;
	substring.length = length;
	memcpy(substring.data, s->data + start, length);
	substring.data[length] = '\0';

	return substring;
}
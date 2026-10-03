#include "../include/m_string.h"
#include <string.h>
#include <stdarg.h>

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
	if (s->data == NULL) return;

	free(s->data);
	s->data = NULL;
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
	if (dest == NULL) return;

	if (dest->data != NULL)
	{
		free(dest->data);
		dest->data = NULL;
	}
	dest->length = 0;

	if (source == NULL || source->data == NULL) return;

	dest->length = source->length;
	dest->data = malloc(source->length + 1);
	memcpy(dest->data, source->data, source->length + 1);
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
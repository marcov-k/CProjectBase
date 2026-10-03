#include "../include/m_string.h"
#include "../include/math_utils.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

static int GrowString(String* s, size_t length)
{
	if (s == NULL) return M_FAILURE_GEN;
	if (s->data != NULL && s->capacity >= length) return M_SUCCESS;

	size_t capacity = max(s->capacity, 1);
	while (capacity < length) capacity *= 2;

	char* newBuffer = realloc(s->data, capacity + 1);
	if (newBuffer == NULL) return M_FAILURE_GEN;

	s->data = newBuffer;
	s->capacity = capacity;

	return M_SUCCESS;
}

String CreateString(const char s[])
{
	String string = { NULL, 0, 0 };

	if (s == NULL)
	{
		if (GrowString(&string, 0) != M_SUCCESS) return string;
		
		string.data[0] = '\0';
		return string;
	}

	size_t length = strlen(s);
	if (GrowString(&string, length) != M_SUCCESS) return string;

	memcpy(string.data, s, length);
	string.data[length] = '\0';
	string.length = length;

	return string;
}

int DestroyString(String* s)
{
	if (s == NULL) return M_FAILURE_GEN;

	if (s->data != NULL) // only free memory if allocated
	{
		free(s->data);
		s->data = NULL;
	}
	s->capacity = 0;
	s->length = 0;

	return M_SUCCESS;
}

int ClearString(String* s)
{
	if (s == NULL) return M_FAILURE_GEN;
	if (s->data == NULL) return M_SUCCESS;

	s->data[0] = '\0';
	s->length = 0;

	return M_SUCCESS;
}

String CopyString(const String* s)
{
	String copy = { NULL, 0, 0 };

	if (s == NULL) return copy;

	if (GrowString(&copy, s->length) != M_SUCCESS) return copy;

	memcpy(copy.data, s->data, s->length);
	copy.data[s->length] = '\0';
	copy.length = s->length;

	return copy;
}

int CopyStringTo(String* dest, const String* source)
{
	if (dest == NULL || source == NULL) return M_FAILURE_GEN;

	if (dest == source) return M_SUCCESS;

	if (source->data == NULL || source->length == 0)
	{
		ClearString(dest);
		return M_SUCCESS;
	}

	if (GrowString(dest, source->length) != M_SUCCESS) return M_FAILURE_GEN;

	memcpy(dest->data, source->data, source->length);
	dest->data[source->length] = '\0';
	dest->length = source->length;

	return M_SUCCESS;
}

int CopyCStrToString(String* dest, const char source[])
{
	if (dest == NULL || source == NULL) return M_FAILURE_GEN;

	size_t length = strlen(source);
	if (length == 0)
	{
		ClearString(dest);
		return M_SUCCESS;
	}

	if (GrowString(dest, length) != M_SUCCESS) return M_FAILURE_GEN;

	memmove(dest->data, source, length);
	dest->data[length] = '\0';
	dest->length = length;

	return M_SUCCESS;
}

StringSplit CreateStringSplit(void)
{
	StringSplit split = { CreateString(NULL), CreateString(NULL) };
	return split;
}

int DestroyStringSplit(StringSplit* split)
{
	if (split == NULL) return M_FAILURE_GEN;

	DestroyString(&split->string1);
	DestroyString(&split->string2);

	return M_SUCCESS;
}

int PrintString(const String* s)
{
	if (s == NULL) return M_FAILURE_GEN;

	printf("%s", s->data);

	return M_SUCCESS;
}

int PrintStringLn(const String* s)
{
	if (s == NULL) return M_FAILURE_GEN;

	printf("%s\n", s->data);

	return M_SUCCESS;
}

static int PrependStringBytes(String* s, const char prepend[], size_t prependLen)
{
	if (s == NULL) return M_FAILURE_GEN;
	if (prepend == NULL || prependLen == 0) return M_SUCCESS;

	int status = M_SUCCESS;
	char* temp = malloc(prependLen);
	if (temp == NULL) goto FAILURE;
	memcpy(temp, prepend, prependLen);

	size_t length = s->length + prependLen;
	if (GrowString(s, length) != M_SUCCESS) goto FAILURE;

	memmove(s->data + prependLen, s->data, s->length);
	memmove(s->data, temp, prependLen);
	s->data[length] = '\0';
	s->length = length;

	goto CLEAN_UP;

FAILURE:
	status = M_FAILURE_GEN;

CLEAN_UP:
	if (temp != NULL) free(temp);
	
	return status;
}

int PrependString(String* s, const String* prepend)
{
	if (prepend == NULL) return M_FAILURE_GEN;
	
	return PrependStringBytes(s, prepend->data, prepend->length);
}

int PrependCStr(String* s, const char prepend[])
{
	if (prepend == NULL) return M_FAILURE_GEN;

	return PrependStringBytes(s, prepend, strlen(prepend));
}

static int AppendStringBytes(String* s, const char append[], size_t appendLen)
{
	if (s == NULL) return M_FAILURE_GEN;
	if (append == NULL || appendLen == 0) return M_SUCCESS;

	int status = M_SUCCESS;
	char* temp = malloc(appendLen);
	if (temp == NULL) goto FAILURE;
	memcpy(temp, append, appendLen);

	size_t length = s->length + appendLen;
	if (GrowString(s, length) != M_SUCCESS) goto FAILURE;

	memmove(s->data + s->length, temp, appendLen);
	s->data[length] = '\0';
	s->length = length;

	goto CLEAN_UP;

FAILURE:
	status = M_FAILURE_GEN;

CLEAN_UP:
	if (temp != NULL) free(temp);

	return status;
}

int AppendString(String* s, const String* append)
{
	if (append == NULL) return M_FAILURE_GEN;

	return AppendStringBytes(s, append->data, append->length);
}

int AppendCStr(String* s, const char append[])
{
	if (append == NULL) return M_FAILURE_GEN;

	return AppendStringBytes(s, append, strlen(append));
}

static int ConcatCStrings(String* dest, const char s1[], size_t s1Len, const char s2[], size_t s2Len)
{
	if (dest == NULL) return M_FAILURE_GEN;

	int status = M_SUCCESS;
	char *temp1 = NULL, *temp2 = NULL;

	if (s1 != NULL && s1Len > 0 && s2 != NULL && s2Len > 0)
	{
		temp1 = malloc(s1Len);
		if (temp1 == NULL) goto FAILURE;
		
		temp2 = malloc(s2Len);
		if (temp2 == NULL) goto FAILURE;

		memcpy(temp1, s1, s1Len);
		memcpy(temp2, s2, s2Len);

		size_t length = s1Len + s2Len;
		if (GrowString(dest, length) != M_SUCCESS) goto FAILURE;

		memcpy(dest->data, temp1, s1Len);
		memcpy(dest->data + s1Len, temp2, s2Len);
		dest->data[length] = '\0';
		dest->length = length;
	}
	else if (s1 != NULL && s1Len > 0)
	{
		temp1 = malloc(s1Len);
		if (temp1 == NULL) goto FAILURE;

		memcpy(temp1, s1, s1Len);

		if (GrowString(dest, s1Len) != M_SUCCESS) goto FAILURE;

		memcpy(dest->data, temp1, s1Len);
		dest->data[s1Len] = '\0';
		dest->length = s1Len;
	}
	else if (s2 != NULL && s2Len > 0)
	{
		temp2 = malloc(s2Len);
		if (temp2 == NULL) goto FAILURE;

		memcpy(temp2, s2, s2Len);

		if (GrowString(dest, s2Len) != M_SUCCESS) goto FAILURE;

		memcpy(dest->data, temp2, s2Len);
		dest->data[s2Len] = '\0';
		dest->length = s2Len;
	}
	else
	{
		ClearString(dest);
	}

	goto CLEAN_UP;

FAILURE:
	status = M_FAILURE_GEN;

CLEAN_UP:
	if (temp1 != NULL) free(temp1);
	if (temp2 != NULL) free(temp2);

	return status;
}

int ConcatStrings(String* dest, const String* s1, const String* s2)
{
	if (dest == NULL || s1 == NULL || s2 == NULL) return M_FAILURE_GEN;

	return ConcatCStrings(dest, s1->data, s1->length, s2->data, s2->length);
}

int ConcatStringCStr(String* dest, const String* s1, const char s2[])
{
	if (dest == NULL || s1 == NULL || s2 == NULL) return M_FAILURE_GEN;

	return ConcatCStrings(dest, s1->data, s1->length, s2, strlen(s2));
}

int ConcatCStrString(String* dest, const char s1[], const String* s2)
{
	if (dest == NULL || s1 == NULL || s2 == NULL) return M_FAILURE_GEN;

	return ConcatCStrings(dest, s1, strlen(s1), s2->data, s2->length);
}

int ReverseString(String* s)
{
	if (s == NULL) return M_FAILURE_GEN;
	if (s->data == NULL || s->length == 0) return M_SUCCESS;

	size_t halfLen = s->length / 2;
	for (size_t i = 0; i < halfLen; ++i)
	{
		size_t otherIndex = s->length - i - 1;
		char temp = s->data[i];
		s->data[i] = s->data[otherIndex];
		s->data[otherIndex] = temp;
	}
	s->data[s->length] = '\0';

	return M_SUCCESS;
}

int SplitString(StringSplit* split, const String* s, size_t index)
{
	if (split == NULL || s == NULL) return M_FAILURE_GEN;

	if (s->data == NULL)
	{
		ClearString(&split->string1);
		ClearString(&split->string2);
		return M_SUCCESS;
	}

	if (index >= s->length)
	{
		if (GrowString(&split->string1, s->length) != M_SUCCESS) return M_FAILURE_GEN;

		ClearString(&split->string2);

		memmove(split->string1.data, s->data, s->length);
		split->string1.data[s->length] = '\0';
		split->string1.length = s->length;
	}
	else if (index == 0)
	{
		if (GrowString(&split->string2, s->length) != M_SUCCESS) return M_FAILURE_GEN;

		ClearString(&split->string1);

		memmove(split->string2.data, s->data, s->length);
		split->string2.data[s->length] = '\0';
		split->string2.length = s->length;
	}
	else
	{
		if (GrowString(&split->string1, index) != M_SUCCESS) return M_FAILURE_GEN;
		if (GrowString(&split->string2, s->length - index) != M_SUCCESS) return M_FAILURE_GEN;

		memmove(split->string1.data, s->data, index);
		split->string1.data[index] = '\0';
		split->string1.length = index;

		memmove(split->string2.data, s->data + index, s->length - index);
		split->string2.data[s->length - index] = '\0';
		split->string2.length = s->length - index;
	}

	return M_SUCCESS;
}

int TrimStringStart(String* s, size_t count)
{
	if (s == NULL) return M_FAILURE_GEN;
	if (s->data == NULL || count == 0) return M_SUCCESS;

	if (count >= s->length)
	{
		ClearString(s);
		return M_SUCCESS;
	}

	memmove(s->data, s->data + count, s->length - count);
	s->data[s->length - count] = '\0';
	s->length -= count;

	return M_SUCCESS;
}

int TrimStringEnd(String* s, size_t count)
{
	if (s == NULL) return M_FAILURE_GEN;
	if (s->data == NULL || count == 0) return M_SUCCESS;

	if (count >= s->length)
	{
		ClearString(s);
		return M_SUCCESS;
	}

	s->data[s->length - count] = '\0';
	s->length -= count;

	return M_SUCCESS;
}

int TrimString(String* s, size_t start, size_t end)
{
	if (s == NULL) return M_FAILURE_GEN;
	if (start > end) return M_FAILURE_GEN;

	if (s->data == NULL) return M_SUCCESS;
	if (start == 0 && end >= s->length) return M_SUCCESS;

	if (start >= s->length)
	{
		ClearString(s);
		return M_SUCCESS;
	}
	end = min(end, s->length);

	size_t length = end - start;
	memmove(s->data, s->data + start, length);
	s->data[length] = '\0';
	s->length = length;

	return M_SUCCESS;
}

int ExtractSubstring(String* dest, const String* s, size_t start, size_t end)
{
	if (dest == NULL || s == NULL) return M_FAILURE_GEN;

	if (start > end) return M_FAILURE_GEN;

	if (s->data == NULL)
	{
		ClearString(dest);
		return M_SUCCESS;
	}
	if (start == 0 && end >= s->length)
	{
		return CopyStringTo(dest, s);
	}

	if (start >= s->length)
	{
		ClearString(dest);
		return M_SUCCESS;
	}

	end = min(end, s->length);

	size_t length = end - start;
	if (GrowString(dest, length) != M_SUCCESS) return M_FAILURE_GEN;

	memmove(dest->data, s->data + start, length);
	dest->data[length] = '\0';
	dest->length = length;

	return M_SUCCESS;
}

bool StringsEqual(const String* s1, const String* s2)
{
	if (s1 == NULL || s2 == NULL) return false;

	if ((s1->data == NULL || s1->length == 0) && (s2->data == NULL || s2->length == 0)) return true;

	if (s1->length != s2->length) return false;

	if (memcmp(s1->data, s2->data, s1->length) == 0) return true;

	return false;
}

bool StringsEqualCStr(const String* s1, const char s2[])
{
	if (s1 == NULL || s2 == NULL) return false;

	size_t s2Len = strlen(s2);
	if ((s1->data == NULL || s1->length == 0) && s2Len == 0) return true;

	if (s1->length != s2Len) return false;

	if (memcmp(s1->data, s2, s1->length) == 0) return true;

	return false;
}

static bool StringContainsBytes(const String* s, const char sub[], size_t subLen)
{
	for (size_t i = 0; i <= s->length - subLen; ++i)
	{
		if (memcmp(s->data + i, sub, subLen) == 0) return true;
	}
	return false;
}

bool ContainsSubstring(const String* s, const String* sub)
{
	if (s == NULL || sub == NULL) return false;

	if (sub->data == NULL || sub->length == 0) return true;
	if (s->data == NULL || s->length == 0) return false;

	if (sub->length > s->length) return false;

	return StringContainsBytes(s, sub->data, sub->length);
}

bool ContainsCStr(const String* s, const char sub[])
{
	if (s == NULL || sub == NULL) return false;

	size_t subLen = strlen(sub);
	if (subLen == 0) return true;
	if (s->data == NULL || s->length == 0) return false;

	if (subLen > s->length) return false;

	return StringContainsBytes(s, sub, subLen);
}
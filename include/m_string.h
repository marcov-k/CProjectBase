#ifndef STRING_H
#define STRING_H

#include <stdlib.h>

typedef struct String
{
	char* data;
	size_t length;
} String;

String CreateString(const char s[]);
void DestroyString(String* s);
String CopyString(const String* s);
void CopyStringTo(String* dest, const String* source);

String ConcatStrings(const String* s1, const String* s2);
String ConcatStringCStr(const String* s1, const char s2[]);
String ConcatCStrString(const char s1[], const String* s2);

#endif
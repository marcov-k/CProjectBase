#ifndef M_STRING_H
#define M_STRING_H

#include <stdlib.h>

typedef struct [[nodiscard("String owns heap memory; the result must be stored and destroyed")]] String
{
	char* data;
	size_t length;
} String;

typedef struct [[nodiscard("StringSplit owns heap memory; the result must be stored and destroyed")]] StringSplit
{
	String string1;
	String string2;
} StringSplit;

// Returned String is owned by and must be destroyed by caller
String CreateString(const char s[]); // create a new String struct from a C-style string
void DestroyString(String* s); // safely frees memory if allocated
// Returned String is owned by and must be destroyed by caller
String CopyString(const String* s); // create a new String struct from an existing String
void CopyStringTo(String* dest, const String* source); // copy data from one String to another
void CopyCStrToString(String* dest, const char source[]); // copy data from C-style string to a String

void DestroyStringSplit(StringSplit* split);

void PrintString(const String* s); // print data of a String
void PrintStringLn(const String* s); // print data of a String and append a line break;

String ConcatStrings(const String* s1, const String* s2); // concatenate two Strings
// Returned String is owned by and must be destroyed by caller
String ConcatStringCStr(const String* s1, const char s2[]); // concatenate a String and a C-style string
// Returned String is owned by and must be destroyed by caller
String ConcatCStrString(const char s1[], const String* s2); // concatenate a C-style string and a String


void ReverseString(String* s); // reverse characters of a String in place
// Returned StringSplit is owned by and must be destroyed caller
StringSplit SplitString(const String* s, size_t index); // split a String at an index
void TrimStringStart(String* s, size_t count); // trim characters from start of a String
void TrimStringEnd(String* s, size_t count); // trim characters from end of a String

#endif
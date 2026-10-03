#ifndef M_STRING_H
#define M_STRING_H

#include <stdlib.h>

typedef struct [[nodiscard("String owns heap memory; the result must be stored and destroyed")]] String
{
	char* data;
	size_t length;
	size_t capacity;
} String;

typedef struct [[nodiscard("StringSplit owns heap memory; the result must be stored and destroyed")]] StringSplit
{
	String string1;
	String string2;
} StringSplit;

/* All functions return empty Strings in case of failure. */

// Create a new String struct from a C-style string
// Returned String is owned by and must be destroyed by caller
String CreateString(const char s[]);
// Free all heap memory used by a String
void DestroyString(String* s);
// Create a new String struct from an existing String
// Returned String is owned by and must be destroyed by caller
String CopyString(const String* s);
// Copy data from one String to another
void CopyStringTo(String* dest, const String* source);
// Copy data from C-style string to a String
void CopyCStrToString(String* dest, const char source[]);

// Free all memory used by a StringSplit
void DestroyStringSplit(StringSplit* split);

// Print data of a String
void PrintString(const String* s);
// Print data of a String and append a line break
void PrintStringLn(const String* s);

// Concatenate two Strings
// Returned String is owned by and must be destroyed by caller
String ConcatStrings(const String* s1, const String* s2);
// Concatenate a String and a C-style string
// Returned String is owned by and must be destroyed by caller
String ConcatStringCStr(const String* s1, const char s2[]);
// Concatenate a C-style string and a String
// Returned String is owned by and must be destroyed by caller
String ConcatCStrString(const char s1[], const String* s2);

// Reverse characters of a String in place
void ReverseString(String* s);
// Split a String at an index
// Returned StringSplit is owned by and must be destroyed caller
StringSplit SplitString(const String* s, size_t index);
// Trim characters from start of a String
void TrimStringStart(String* s, size_t count);
// Trim characters from end of a String
void TrimStringEnd(String* s, size_t count);
// Trim characters from start and end of a String (start index inclusive, end index exclusive)
void TrimString(String* s, size_t start, size_t end);
// Extract substring from a String between a start and index index (start inclusive, end exclusive)
// Returned String is owned by and must be destroyed by caller
String ExtractSubstring(const String* s, size_t start, size_t end);

#endif
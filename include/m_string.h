#ifndef M_STRING_H
#define M_STRING_H

#include "m_result.h"
#include <stdlib.h>
#include <stdbool.h>

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

/*
	Empty vs. NULL:
	An empty String is defined as having a length of 0, regardless of data buffer allocation state.
	Functions will treat empty Strings with NULL and non-NULL data buffers as equivalent, except when
	returning a new String under failure conditions wherein a NULL data buffer will be returned.


	Copies:
	Shallow copies are not supported under any conditions.

	
	Valid Strings:
	Every String must be initialized (zeroed or created) before use.
	Valid Strings obey the following invariants:
		- NULL data implies 0 length and 0 capacity
		- Length never exceeds capacity
		- A non-NULL buffer is always terminated at length
	

	Return Values and Failure Guarantees:
	A return value of M_SUCCESS (0) indicates success; M_FAILURE_GEN (1) indicates failure.
	For functions returning an index, the index of the null terminator (length + 1) will be returned
	in case of a failure of any kind (eg. a substring not being present, etc.).

	A failure is defined as any function and/or operation not succeeding completely.

	All functions returning new structs, eg. a new String, will return an empty struct with a NULL data buffer in case of failure;
	if the function succeeds but results in an empty struct, the struct will have a non-NULL data buffer.
	Functions mutating parameters will leave output parameters unmodified in case of failure.


	NULL and Empty Arguments:
	NULL pointer arguments to functions returning Strings will return an empty String with a NULL data buffer.
	Any NULL pointer argument to a function mutating output parameters will result in a failure.
	NULL pointer arguments to comparison functions will always cause the function to return false.

	Functions receiving empty Strings as parameters will execute normally.


	Aliasing:
	Argument aliasing is supported for the following operations:
		- CopyTo self
		- Prepend self
		- Append self
		- Concat with destination equal to an operand
		- Extract substring into the same String
		- Split String aliasing a member String of the given StringSplit

	Other aliasing cases should be assumed unsupported.


	Bounds Checking:
	Trimming and extracting functions will clamp indices within the length of the String.
	Data access functions, eg. CharAt(), will return an error code of M_FAILURE_GEN (1) in case of an out-of-range index.
	Out-of-order index arguments, eg. start > end, will return an error code of M_FAILURE_GEN (1).
*/

// Create a new String struct from a C-style string
// Null pointer argument will return an empty String
// Returned String is owned by and must be destroyed by caller
String CreateString(const char s[]);
// Free all heap memory used by a String
int DestroyString(String* s);
// Clears the data of a String
int ClearString(String* s);
// Create a new String struct from an existing String
// Returned String is owned by and must be destroyed by caller
String CopyString(const String* s);
// Copy data from one String to another
int CopyStringTo(String* dest, const String* source);
// Copy data from C-style string to a String
// Source C-strings points into the dest String are unsupported
int CopyCStrToString(String* dest, const char source[]);

// Create a new zero-initialized StringSplit struct
// Returned StringStruct is owned by an must be destroyed by caller
StringSplit CreateStringSplit(void);
// Free all memory used by a StringSplit
int DestroyStringSplit(StringSplit* split);

// Print data of a String
int PrintString(const String* s);
// Print data of a String and append a line break
int PrintStringLn(const String* s);

// Prepend a String to another String
int PrependString(String* s, const String* prepend);
// Prepend a C-string to a String
int PrependCStr(String* s, const char prepend[]);
// Append a String to another String
int AppendString(String* s, const String* append);
// Append a C-string to a String
int AppendCStr(String* s, const char append[]);

// Concatenate two Strings
int ConcatStrings(String* dest, const String* s1, const String* s2);
// Concatenate a String and a C-style string
int ConcatStringCStr(String* dest, const String* s1, const char s2[]);
// Concatenate a C-style string and a String
int ConcatCStrString(String* dest, const char s1[], const String* s2);

// Reverse characters of a String in place
int ReverseString(String* s);
// Split a String at an index
// The provided StringSplit must have valid initialization - either from CreateStringSplit() or a previous SplitString() call
int SplitString(StringSplit* split, const String* s, size_t index);
// Trim characters from start of a String
int TrimStringStart(String* s, size_t count);
// Trim characters from end of a String
int TrimStringEnd(String* s, size_t count);
// Trim characters from start and end of a String (start index inclusive, end index exclusive)
int TrimString(String* s, size_t start, size_t end);
// Extract substring from a String between a start and end index (start inclusive, end exclusive)
int ExtractSubstring(String* dest, const String* s, size_t start, size_t end);

// Check whether two Strings are exactly equal
bool StringsEqual(const String* s1, const String* s2);
// Check whether a String and a C-string are exactly equal
bool StringsEqualCStr(const String* s1, const char s2[]);
// Check whether a String contains another String
bool ContainsSubstring(const String* s, const String* sub);
// Check whether a String contains a C-string
bool ContainsCStr(const String* s, const char sub[]);

#endif
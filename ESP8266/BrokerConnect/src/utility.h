#include <WString.h>
#include <stdio.h>

String CreateJson(String* keys, String* values, int count);
String CreateJson(String* keys, String* values, bool* isString, int count);
String CreateJson(String* keys, float* values, int count);
String* SplitString(char* message);

#include <WString.h>
#include <stdio.h>

char* CreateJson(String* keys, String* values, int count);
char* CreateJson(String accountId, String espId, String deviceId, String status, String data);
char* CreateJson(String* keys, float* values, int count);
String* SplitString(char* message);

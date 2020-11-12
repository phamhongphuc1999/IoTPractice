#include "utility.h"

char* CreateJson(String* keys, String* values, int count){
    String result = "{";
    for (int i = 0; i < count - 1; i++)
        result += "\"" + keys[i] + "\":\"" + values[i] + "\",";
    result += "\"" + keys[count - 1] + "\":\"" + values[count - 1] + "\"}";
    int length = result.length();
    char* char_array = new char[length];
    strcpy(char_array, result.c_str());
    return char_array;
}

char* CreateJson(String* keys, float* values, int count){
    String result = "{";
    for(int i = 0; i < count - 1; i++)
        result += "\"" + keys[i] + "\":\"" + values[i] + "\",";
    result += "\"" + keys[count - 1] + "\":\"" + values[count - 1] + "\"}";
    int length = result.length();
    char* char_array = new char[length];
    strcpy(char_array, result.c_str());
    return char_array;
}

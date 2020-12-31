#include "utility.h"

String CreateJson(String* keys, String* values, int count){
    String result = "{";
    for (int i = 0; i < count - 1; i++)
        result += "\"" + keys[i] + "\":\"" + values[i] + "\",";
    result += "\"" + keys[count - 1] + "\":\"" + values[count - 1] + "\"}";
    int length = result.length();
    char* char_array = new char[length];
    strcpy(char_array, result.c_str());
    return char_array;
}

String CreateJson(String* keys, String* values, bool* isString, int count){
    String result = "{";
    for (int i = 0; i < count - 1; i++){
        if(isString[i]) result += "\"" + keys[i] + "\":\"" + values[i] + "\",";
        else result += "\"" + keys[i] + "\":" + values[i] + ",";
    }
    if(isString[count - 1]) result += "\"" + keys[count - 1] + "\":\"" + values[count - 1] + "\"}";
    else result += "\"" + keys[count - 1] + "\":" + values[count - 1] + "}";
    int length = result.length();
    char* char_array = new char[length];
    strcpy(char_array, result.c_str());
    return char_array;
}

String CreateJson(String* keys, float* values, int count){
    String result = "{";
    for(int i = 0; i < count - 1; i++)
        result += "\"" + keys[i] + "\":\"" + values[i] + "\",";
    result += "\"" + keys[count - 1] + "\":\"" + values[count - 1] + "\"}";
    int length = result.length();
    char* char_array = new char[length];
    strcpy(char_array, result.c_str());
    return char_array;
}

String* SplitString(char* message){
    String* result = new String[4];
    int count = 0;
    char* temp = strtok(message, ".");
    while(count < 3) {
        result[count] = temp;
        temp = strtok(NULL, ".");
        count++;
    }
    return result;
}

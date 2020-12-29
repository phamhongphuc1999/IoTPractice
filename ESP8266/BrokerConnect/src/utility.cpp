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

char* CreateJson(String accountId, String espId, String deviceId, String status, String data){
    String result = "{";
    result += "\"homeID\":\"" + accountId + "\",";
    result += "\"espID\":\"" + espId + "\",";
    result += "\"deviceID\":\"" + deviceId + "\",";
    result += "\"status\":\"" + status + "\",";
    result += "\"data\":" + data + "}";
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

String* SplitString(char* message){
    String* result = new String[4];
    int count = 0;
    char* temp = strtok(message, ".");
    while(count < 4) {
        result[count] = temp;
        temp = strtok(NULL, ".");
        count++;
    }
    return result;
}

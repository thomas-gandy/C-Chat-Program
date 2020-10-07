#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "myString.h"


char* myString_new(struct myString* stringObj, const char* newString) {
    unsigned long stringSize = strlen(newString) + 1;
    (*stringObj).string = (char*) malloc(sizeof(char) * stringSize * HEAP_MULTIPLIER);
    if (!(*stringObj).string) return NULL;

    (*stringObj).currentStringSize = stringSize - 1;    // subtract the null char
    (*stringObj).currentHeapSize = sizeof(char) * stringSize * HEAP_MULTIPLIER;
    strncpy((*stringObj).string, newString, stringSize);
    return (*stringObj).string;
}

char* myString_cat(struct myString* stringObj, const char* newString) {
    unsigned long stringSize = strlen(newString);
    unsigned long combinedStringSize = (*stringObj).currentStringSize + stringSize;
    if (combinedStringSize + 1 > (*stringObj).currentHeapSize) {    // add null char
        char* temp = (char*) malloc(sizeof(char) * (combinedStringSize + 1) * HEAP_MULTIPLIER); // add null char
        int tempIndex = 0;
        for (unsigned long i = 0; i < (*stringObj).currentStringSize; i++) {
            temp[tempIndex] = (*stringObj).string[i];
            tempIndex++;
        }
        for (unsigned long i = 0; i < stringSize; i++) {
            temp[tempIndex] = newString[i];
            tempIndex++;
        }
        temp[tempIndex] = '\0';
        (*stringObj).string = temp;
        (*stringObj).currentHeapSize = sizeof(char) * (combinedStringSize + 1) * HEAP_MULTIPLIER;
    }
    else {
        int tempIndex = (*stringObj).currentStringSize;
        for (unsigned long i = 0; i < stringSize; i++) {
            (*stringObj).string[tempIndex] = newString[i];
            tempIndex++;
        }
        (*stringObj).string[tempIndex] = '\0';
    }
    (*stringObj).currentStringSize = combinedStringSize;

    return (*stringObj).string;
}

char* myString_catn(struct myString* stringObj, const char* newString, unsigned long bytes) {
    unsigned long stringSize = strlen(newString);
    unsigned long combinedStringSize = (*stringObj).currentStringSize + bytes;
    if (combinedStringSize > (*stringObj).currentHeapSize) {
        char* temp = (char*) malloc(sizeof(char) * combinedStringSize * HEAP_MULTIPLIER);
        int tempIndex = 0;
        for (unsigned long i = 0; i < (*stringObj).currentStringSize; i++) {
            temp[tempIndex] = (*stringObj).string[i];
            tempIndex++;
        }
        for (unsigned long i = 0; i < bytes; i++) {
            temp[tempIndex] = newString[i];
            tempIndex++;
        }
        (*stringObj).string = temp;
        (*stringObj).currentHeapSize = sizeof(char) * combinedStringSize * HEAP_MULTIPLIER;
    }
    else {
        int tempIndex = (*stringObj).currentStringSize;
        for (unsigned long i = 0; i < bytes; i++) {
            (*stringObj).string[tempIndex] = newString[i];
            tempIndex++;
        }
    }
    (*stringObj).currentStringSize = combinedStringSize;

    return (*stringObj).string;
}

char* myString_setchar(struct myString* stringObj, const unsigned long index, const char character) {
    if (index >= (*stringObj).currentStringSize) return NULL;
    (*stringObj).string[index] = character;

    return &((*stringObj).string[index]);
}

void myString_free(struct myString* stringObj) {
    if ((*stringObj).string) free((*stringObj).string);
}

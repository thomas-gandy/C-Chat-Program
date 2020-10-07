#ifndef MY_STRING_H
#define MY_STRING_H

#define HEAP_MULTIPLIER 2

struct myString {
    char* string;
    unsigned long currentStringSize;
    unsigned long currentHeapSize;
};

// create a new string for struct, ensure the struct has already been freed first
char* myString_new(struct myString* stringObj, const char* newString);
// concat the new string to the stringObj
char* myString_cat(struct myString* stringObj, const char* newString);
// concat n bytes of the new string to the stringObj
char* myString_catn(struct myString* stringObj, const char* newString, unsigned long bytes);
// set a specific character in the stringObj
char* myString_setchar(struct myString* stringObj, const unsigned long index, const char character);
// free dynamic memory of stringObj
void myString_free(struct myString* stringObj);

#endif

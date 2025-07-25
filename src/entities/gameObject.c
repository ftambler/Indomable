#include "gameObject.h"
#include "string.h"

int getEnumOfType(char* value) {
    if(strcmp(value, "OBJECT") == 0) return OBJECT;
    if(strcmp(value, "SPAWN") == 0) return SPAWN;
    if(strcmp(value, "CHECKPOINT") == 0) return CHECKPOINT;
    if(strcmp(value, "NEXT") == 0) return NEXT;
    if(strcmp(value, "WIN") == 0) return WIN;
    return -1;
}
#include "gameObject.h"
#include "string.h"

#include "config.h"

// #include <stdio.h>

int getEnumOfType(char* value) {
    if(strcmp(value, "OBJECT") == 0) return OBJECT;
    if(strcmp(value, "SPAWN") == 0) return SPAWN;
    if(strcmp(value, "CHECKPOINT") == 0) return CHECKPOINT;
    if(strcmp(value, "NEXT") == 0) return NEXT;
    if(strcmp(value, "WIN") == 0) return WIN;
    return -1;
}

Rectangle getObjectHitBox(const GameObject* gameObject) {
    // printf("%f %f %d %d \n", gameObject->position.x, gameObject->position.y, gameObject->object.size, gameObject->object.size);
    return (Rectangle){gameObject->position.x * tileSize, gameObject->position.y * tileSize, gameObject->object.size, gameObject->object.size};
}
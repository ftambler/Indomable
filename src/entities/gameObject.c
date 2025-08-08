#include "gameObject.h"
#include "string.h"

#include "config.h"

int getEnumOfType(char* value) {
    if(strcmp(value, "OBJECT") == 0) return OBJECT;
    if(strcmp(value, "SPAWN") == 0) return SPAWN;
    if(strcmp(value, "CHECKPOINT") == 0) return CHECKPOINT;
    if(strcmp(value, "NEXT") == 0) return NEXT;
    if(strcmp(value, "WIN") == 0) return WIN;
    return -1;
}

Rectangle getObjectHitBox(const GameObject* gameObject) {
    return (Rectangle) {
        gameObject->position.x * tileSize, 
        gameObject->position.y * tileSize, 
        gameObject->object.size.x * tileSize, 
        gameObject->object.size.y * tileSize
    };
}
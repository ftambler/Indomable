#pragma once

#include "block.h"
#include "raylib.h"

typedef enum { OBJECT, SPAWN, NEXT, WIN } Type;

typedef struct {
    Vector2 position;
    Type type;
    union {
        Block object;
    };
} GameObject;

int getEnumOfType(char* value);
#pragma once

#include "raylib.h"
#include "block.h"
#include "checkpoint.h"

typedef enum { OBJECT, SPAWN, CHECKPOINT, NEXT, WIN } Type;

typedef struct {
    Vector2 position;
    Type type;
    union {
        Block object;
        Checkpoint checkpoint;
    };
} GameObject;

int getEnumOfType(char* value);
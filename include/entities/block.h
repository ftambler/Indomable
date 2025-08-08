#pragma once

#include <stdbool.h>

typedef enum { GRASS, DEFAULT } TextureId;
typedef struct {
    TextureId textureId;
    Vector2 size;
    float scale;
} Block;

int getEnumOfTexture(char* value);
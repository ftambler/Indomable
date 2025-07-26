#pragma once

#include <stdbool.h>

typedef enum { GRASS, DEFAULT } TextureId;
typedef struct {
    TextureId textureId;
    int size;
} Block;

int getEnumOfTexture(char* value);
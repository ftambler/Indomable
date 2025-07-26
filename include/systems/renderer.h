#pragma once

#include "raylib.h"
#include "level.h"

extern int tileSize;

void initRenderer();
void deInitRenderer();

void drawPlayer(Vector2 position, int playerSize);
void drawBlock(Texture2D, Vector2 position, int size);

void drawLevel(Level* level);
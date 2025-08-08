#pragma once

#include "raylib.h"
#include "level.h"

void initRenderer();
void deInitRenderer();

void drawLevel(Level* level);
void drawPlayer(Vector2 position, int playerSize);
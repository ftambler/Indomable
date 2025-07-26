#pragma once

#include "raylib.h"
#include "level.h"

void initRenderer();
void deInitRenderer();

void drawPlayer(Vector2 position);

void drawLevel(Level* level);
#pragma once

#include "raylib.h"

// Consts
extern float gravity;
extern float groundFriction;

bool checkSqSqCollision(Vector2 obj1, int size1, Vector2 obj2, int size2);
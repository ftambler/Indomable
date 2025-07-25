#pragma once

#include "raylib.h"

typedef struct {
    Vector2 position;
    Vector2 velocity;
    int size;
    bool isGrounded;
    bool isAlive;
    float jumpForce;
    float moveSpeed;
} Player;

void initPlayer(Player* player);

void spawnPlayer(Player* player, int x, int y);

void killPlayer(Player* player);

void handlePlayerCollision(Vector2 *playerPos, Vector2 *playerVel, int playerSize, bool *grounded, Vector2 obj, int objSize);
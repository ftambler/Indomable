#pragma once

#include "raylib.h"

#include "inputs.h"

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

void handlePlayerInput(Player* player);

void handlePlayerPhysics(Player* player, float deltaTime);

Rectangle getPlayerHitBox(const Player* player);
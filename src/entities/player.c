#include "player.h"

#include <math.h>

#include "physics.h"
#include "inputs.h"
#include "config.h"

void initPlayer(Player* player) {
    player->velocity = (Vector2){0, 0};
    player->size = 32;
    player->isGrounded = false;
    player->isAlive = false;
    player->moveSpeed = 250;
    player->jumpForce = 300;
}

void spawnPlayer(Player* player, int x, int y) {
    player->position = (Vector2){x, y};
    player->velocity = (Vector2){0, 0};
    player->isAlive = true;
}

void killPlayer(Player* player) {
    player->isAlive = false;
    // TODO Animation
}

void handlePlayerCollision(Player* player, Rectangle objBounds) {
    // Compute overlaps
    float overlapX = (player->position.x + player->size) - objBounds.x;
    float overlapX2 = (objBounds.x + objBounds.width) - player->position.x;
    float overlapY = (player->position.y + player->size) - objBounds.y;
    float overlapY2 = (objBounds.y + objBounds.height) - player->position.y;


    // Determine minimal overlap on each axis
    float minX = (overlapX < overlapX2) ? overlapX : -overlapX2;
    float minY = (overlapY < overlapY2) ? overlapY : -overlapY2;

    // Choose the axis with shallower push-out
    if (fabsf(minX) < fabsf(minY)) {
        // Horizontal collision
        player->position.x -= minX;
        player->velocity.x = 0;
    } else {
        // Vertical collision
        player->position.y -= minY;
        player->velocity.y = 0;
        if (minY > 0) {
            player->isGrounded = true; // landed on top
        }
    }
}

void handlePlayerInput(Player* player) {
    if(IsKeyDown(RESET) || player->position.y > GetScreenHeight() * 1.5f) player->isAlive = false;
    if(IsKeyDown(MOVE_RIGHT)) player->velocity.x += player->moveSpeed;
    if(IsKeyDown(MOVE_LEFT))  player->velocity.x -= player->moveSpeed;
    if(IsKeyPressed(JUMP) && player->isGrounded) {
        player->velocity.y = -player->jumpForce;
        player->isGrounded = false;
    }
}

void handlePlayerPhysics(Player* player, float deltaTime) {
    // Gravity
    player->velocity.y += gravity * deltaTime;
    // Friction
    player->velocity.x *= groundFriction;
    // Position
    player->position.x += player->velocity.x * deltaTime;
    player->position.y += player->velocity.y * deltaTime;
}

Rectangle getPlayerHitBox(const Player* player) {
    return (Rectangle){player->position.x, player->position.y, player->size, player->size};
}
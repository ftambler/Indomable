#include <math.h>
#include "player.h"

void InitPlayer(Player* player) {
    player->position = (Vector2){0, 0};
    player->velocity = (Vector2){0, 0};
    
    player->size = 32;
    player->isGrounded = true;
    player->moveSpeed = 5;
    player->jumpForce = 5;
}

void handlePlayerCollision(Vector2 *playerPos, Vector2 *playerVel, int playerSize, bool *grounded, Vector2 obj, int objSize) {
    // Compute overlaps
    float overlapX = (playerPos->x + playerSize) - (obj.x * objSize);
    float overlapX2 = ((obj.x * objSize) + objSize) - playerPos->x;
    float overlapY = (playerPos->y + playerSize) - (obj.y * objSize);
    float overlapY2 = ((obj.y * objSize) + objSize) - playerPos->y;

    // Determine minimal overlap on each axis
    float minX = (overlapX < overlapX2) ? overlapX : -overlapX2;
    float minY = (overlapY < overlapY2) ? overlapY : -overlapY2;

    // Choose the axis with shallower push-out
    if (fabsf(minX) < fabsf(minY)) {
        // Horizontal collision
        playerPos->x -= minX;
        playerVel->x = 0;
    } else {
        // Vertical collision
        playerPos->y -= minY;
        playerVel->y = 0;
        if (minY > 0) {
            *grounded = true; // landed on top
        }
    }
}
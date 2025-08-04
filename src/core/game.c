#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "player.h"
#include "loader.h"
#include "physics.h"
#include "renderer.h"
#include "camera.h"

// Global
Level* levelArray;
GameObject* roomObjects;
GameObject* activeCheckpoint;
int currentLevel;
int level_count;
Player player;

// Controls
static const int MOVE_LEFT = KEY_LEFT;
static const int MOVE_RIGHT = KEY_RIGHT;
static const int JUMP = KEY_UP;
static const int RESET = KEY_R;
// static const int PAUSE = KEY_P;

Camera2D camera = {0};

void initGame() {
    initCamera(&camera, player.position);

    loadLevel(&level_count, &levelArray, &activeCheckpoint);
    initRenderer();

    initPlayer(&player);

    currentLevel = 0;
    roomObjects = levelArray[currentLevel].objects;
}

void deInitGame() {
    deInitRenderer();
    for(int i = 0; i < level_count; i++) free(levelArray[i].objects);
    free(levelArray);
}

void updateGameScreen() {
    updateGame(GetFrameTime());
    drawGame();
}

void updateGame(float deltaTime) {
    if (!player.isAlive) spawnPlayer(&player, activeCheckpoint->position.x * tileSize, activeCheckpoint->position.y * tileSize);

    // Input (TODO VOID DEATH)
    if(IsKeyDown(RESET) || player.position.y > GetScreenHeight() * 1.5f) player.isAlive = false;
    if(IsKeyDown(MOVE_RIGHT)) player.velocity.x += player.moveSpeed;
    if(IsKeyDown(MOVE_LEFT))  player.velocity.x -= player.moveSpeed;
    if(IsKeyPressed(JUMP) && player.isGrounded) {
        player.velocity.y = -player.jumpForce;
        player.isGrounded = false;
    }

    // Gravity
    player.velocity.y += gravity * deltaTime;
    // Friction
    player.velocity.x *= groundFriction;
    // Position
    player.position.x += player.velocity.x * deltaTime;
    player.position.y += player.velocity.y * deltaTime;

    updateCamera(&camera, player.position);

    // GameObject Collisions
    for(int i = 0; i < levelArray[currentLevel].objectCount; i++) {
        switch (roomObjects[i].type) {
            case OBJECT:
                if(checkSqSqCollision(player.position, player.size, roomObjects[i].position, tileSize))
                handlePlayerCollision(&player.position, &player.velocity, player.size, &player.isGrounded, roomObjects[i].position, tileSize);
                break;
                
            case CHECKPOINT:
                if(checkSqSqCollision(player.position, player.size, roomObjects[i].position, tileSize) && !roomObjects[i].checkpoint.isActive) {
                    setCheckpointInactive(&activeCheckpoint->checkpoint);
                    activeCheckpoint = &roomObjects[i];
                    setCheckpointActive(&activeCheckpoint->checkpoint);
                } 
                break;

            default:
                break;
        }

    }

}

char level[8];
void drawGame(void) {
    BeginDrawing();
    
    ClearBackground(RAYWHITE);
    
    // Current Level
    sprintf(level, "Level %d", currentLevel);
    DrawText(level, 20, 20, 20, BLACK);

    BeginMode2D(camera);

        drawLevel(&(levelArray[currentLevel]));

        drawPlayer(player.position, player.size);

    EndMode2D();

    EndDrawing();
}
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "player.h"
#include "loader.h"
#include "physics.h"
#include "renderer.h"
#include "camera.h"
#include "button.h"

// Level
Level* levelArray;
GameObject* roomObjects;
GameObject* activeCheckpoint;
int currentLevel;
int level_count;
Player player;
Camera2D camera;

// Controls
static const int MOVE_LEFT = KEY_LEFT;
static const int MOVE_RIGHT = KEY_RIGHT;
static const int JUMP = KEY_UP;
static const int RESET = KEY_R;
static const int PAUSE = KEY_P;

// Misc
bool paused;

Button titleScreenButton = {
    .bounds = { 100, 100, 200, 50 },
    .text = "Exit to title Screen",
    .bgColor = GRAY,
    .hoverColor = DARKGRAY,
    .textColor = WHITE,
};

bool exitGameClicked = false;

void initGame() {
    paused =
    loadLevel(&level_count, &levelArray, &activeCheckpoint);
    initRenderer();
    
    initCamera(&camera, player.position);
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
    if(paused) updatePauseMenu();
    else updateGame(GetFrameTime());
    
    BeginDrawing();
        drawGame();
        if(paused) drawPausedMenuOverLay();
    EndDrawing();

}

void updateGame(float deltaTime) {    
    if(paused) return;

    if (!player.isAlive) spawnPlayer(&player, activeCheckpoint->position.x * tileSize, activeCheckpoint->position.y * tileSize);

    // Input (TODO VOID DEATH)
    if(IsKeyDown(RESET) || player.position.y > GetScreenHeight() * 1.5f) player.isAlive = false;
    if(IsKeyDown(MOVE_RIGHT)) player.velocity.x += player.moveSpeed;
    if(IsKeyDown(MOVE_LEFT))  player.velocity.x -= player.moveSpeed;
    if(IsKeyPressed(JUMP) && player.isGrounded) {
        player.velocity.y = -player.jumpForce;
        player.isGrounded = false;
    }
    if(IsKeyDown(PAUSE)) paused = true;

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

void drawGame(void) {
    char level[8];
    
    // TODO BACKGROUND
    ClearBackground(RAYWHITE);
    
    // Current Level
    sprintf(level, "Level %d", currentLevel);
    DrawText(level, 20, 20, 20, BLACK);

    BeginMode2D(camera);

        drawLevel(&(levelArray[currentLevel]));

        drawPlayer(player.position, player.size);

    EndMode2D();
}

void drawPausedMenuOverLay() {
    DrawRectangle(0,0,GetScreenWidth(), GetScreenHeight(), (Color){40,40,40,128});

    drawButton(&titleScreenButton);
}

void updatePauseMenu() {
    updateButton(&titleScreenButton);
    if(isButtonClicked(&titleScreenButton)) {
        exitGameClicked = true;
    }
}

bool shouldStopGame() {
    bool res = exitGameClicked;
    exitGameClicked = false;
    return res;
}
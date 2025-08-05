#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "player.h"
#include "loader.h"
#include "physics.h"
#include "renderer.h"
#include "camera.h"
#include "button.h"
#include "inputs.h"

// Level
Level* levelArray;
GameObject* roomObjects;
GameObject* activeCheckpoint;
int currentLevel;
int level_count;
Player player;
Camera2D camera;

// Misc
bool paused;

Button titleScreenButton;
Button unpauseButton;

bool exitGameClicked = false;

void initGame() {
    paused = false;
    loadLevel(&level_count, &levelArray, &activeCheckpoint);
    
    initCamera(&camera, player.position);
    initPlayer(&player);
    initPauseMenuButtons();

    initRenderer();

    currentLevel = 0;
    roomObjects = levelArray[currentLevel].objects;
}

void initPauseMenuButtons() {
    Rectangle pos = {(GetScreenWidth() / 2) - 100, (GetScreenHeight() / 2) - 40, 200, 80};
    initButton(&unpauseButton, pos, "Resume", GRAY, DARKGRAY, WHITE);
    pos.y += 120;
    initButton(&titleScreenButton, pos, "Exit to Menu", GRAY, DARKGRAY, WHITE);
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

    // Input
    handlePlayerInput(&player);

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
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){50, 50, 50, 128});

    drawButton(&unpauseButton);
    drawButton(&titleScreenButton);
}

void updatePauseMenu() {
    updateButton(&titleScreenButton);
    updateButton(&unpauseButton);

    if(isButtonClicked(&titleScreenButton)) exitGameClicked = true;
    if(isButtonClicked(&unpauseButton)) paused = false;
}

bool shouldStopGame() {
    bool res = exitGameClicked;
    exitGameClicked = false;
    return res;
}
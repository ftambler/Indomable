#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include "pauseOverlay.h"
#include "renderer.h"
#include "level.h"
#include "levelLoader.h"
#include "physics.h"
#include "camera.h"
#include "player.h"
#include "gameObject.h"
#include "config.h"
#define PAUSE KEY_P

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

bool exitGameClicked = false;

void initGame() {
    paused = false;
    loadLevel(&level_count, &levelArray, &activeCheckpoint);
    
    initCamera(&camera, player.position);
    initPlayer(&player);
    initPauseOverlay();

    initRenderer();

    currentLevel = 0;
    roomObjects = levelArray[currentLevel].objects;
}

void deInitGame() {
    deInitRenderer();
    for(int i = 0; i < level_count; i++) free(levelArray[i].objects);
    free(levelArray);
}

void updateGameScreen() {
    if(paused) updatePauseOverlay(&paused, &exitGameClicked);
    else updateGame(GetFrameTime());
    
    BeginDrawing();
        drawGame();
        if(paused) drawPauseOverlay();
    EndDrawing();

}

void updateGame(float deltaTime) {    
    // Pause 
    if(paused) return;
    if(IsKeyDown(PAUSE)) paused = true;

    // Respawn (TODO ANIMATION)
    if(!player.isAlive) 
        spawnPlayer(&player, activeCheckpoint->position.x * tileSize, activeCheckpoint->position.y * tileSize);
    
    handlePlayerInput(&player);
    handlePlayerPhysics(&player, deltaTime);

    updateCamera(&camera, player.position);

    // GameObject Collisions
    Rectangle playerHitbox = getPlayerHitBox(&player);
    for(int i = 0; i < levelArray[currentLevel].objectCount; i++) {
        switch (roomObjects[i].type) {
            case OBJECT:
                Rectangle objectHitbox = getObjectHitBox(&roomObjects[i]);
                if(CheckCollisionRecs(playerHitbox, objectHitbox))
                    handlePlayerCollision(&player, objectHitbox);
                break;
                
            case CHECKPOINT:
                Rectangle checkpointHitbox = getNonObjectHitBox(&roomObjects[i]);
                if(CheckCollisionRecs(playerHitbox, checkpointHitbox) && !roomObjects[i].checkpoint.isActive) {
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

bool shouldStopGame() {
    bool res = exitGameClicked;
    exitGameClicked = false;
    return res;
}
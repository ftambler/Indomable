#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "player.h"
#include "loader.h"
#include "physics.h"


//TODO
int screenHeight = 680;
int tileSize = 20;

// Global
Level* levelArray;
GameObject* roomObjects;
int currentLevel;
int level_count;
Player player;

// Controls
static const int MOVE_LEFT = KEY_LEFT;
static const int MOVE_RIGHT = KEY_RIGHT;
static const int JUMP = KEY_UP;
static const int RESET = KEY_R;
// static const int PAUSE = KEY_P;

// Texture
Texture2D playerSprite;
Texture2D grassSprite;

void initGame() {
    loadTextures();
    loadLevel(&level_count, &levelArray);
        
    initPlayer(&player);

    roomObjects = levelArray[currentLevel].objects;
}


void deInitGame() {
    UnloadTexture(playerSprite);
    UnloadTexture(grassSprite);
    for(int i = 0; i < level_count; i++) free(levelArray[i].objects);
    free(levelArray);
}

void loadTextures() {    
    playerSprite = LoadTexture("assets/textures/player.png");
    grassSprite = LoadTexture("assets/textures/grass.png");
}

void updateGame(float deltaTime) {
    // Input
    if(IsKeyDown(RESET)) player.isAlive = false;
    if(IsKeyDown(MOVE_RIGHT)) player.velocity.x += player.moveSpeed;
    if(IsKeyDown(MOVE_LEFT))  player.velocity.x -= player.moveSpeed;
    if(IsKeyPressed(JUMP) && player.isGrounded) {
        player.velocity.y = -player.jumpForce;
        player.isGrounded = false;
    }

    // Gravity
    player.velocity.y += gravity * deltaTime;

    // Position
    player.position.x += player.velocity.x * deltaTime;
    player.position.y += player.velocity.y * deltaTime;

    // Collision (ground)
    if (player.position.y + player.size > screenHeight) {
        player.position.y = screenHeight - player.size;
        player.velocity.y = 0;
        player.isGrounded = true;
    }
    for(int i = 0; i < levelArray[currentLevel].objectCount; i++) {
        switch (roomObjects[i].type) {
        case OBJECT:
            if(checkSqSqCollision(player.position, player.size, roomObjects[i].position, tileSize))
                handlePlayerCollision(&player.position, &player.velocity, player.size, &player.isGrounded, roomObjects[i].position, tileSize);

            break;
        case SPAWN:
            if(!player.isAlive) spawnPlayer(&player, roomObjects[i].position.x * tileSize, roomObjects[i].position.y * tileSize);

            break;
        default:
            break;
        }

    }

    // Friction
    player.velocity.x *= groundFriction;
}

char level[8];
void drawGame(void) {

    BeginDrawing();
    
    ClearBackground(RAYWHITE);
    
    sprintf(level, "Level %d", currentLevel);
    DrawText(level, 0, 0, 20, BLACK);

    for(int i = 0; i < levelArray[currentLevel].objectCount; i++) {
        // DrawRectangle(roomObjects[i].position.x * tileSize, roomObjects[i].position.y * tileSize, tileSize, tileSize, BLACK);
        if(roomObjects[i].type != OBJECT) continue;

        Texture2D drawTexture;
        switch (roomObjects[i].object.textureId)
        {
        case GRASS:
            drawTexture = grassSprite;
            break;
        
        default:
            drawTexture = grassSprite;
            break;
        }
        
        DrawTexturePro(drawTexture, (Rectangle){ 0.0f, 0.0f, grassSprite.height, grassSprite.width }, 
            (Rectangle){ roomObjects[i].position.x * tileSize, roomObjects[i].position.y * tileSize, tileSize, tileSize }, 
            (Vector2){0, 0}, 0.0f, WHITE);

    }

    // DrawTexture(playerSprite, player.position.x, player.position.y, WHITE);
    DrawTexturePro(playerSprite, (Rectangle){ 0.0f, 0.0f, playerSprite.height, playerSprite.width }, 
        (Rectangle){ player.position.x, player.position.y, player.size, player.size }, 
        (Vector2){0, 0}, 0.0f, WHITE);

    EndDrawing();
}
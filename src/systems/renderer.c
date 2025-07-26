#include <stdio.h>
#include "renderer.h"

Texture2D playerSprite;
Texture2D grassSprite;

void initRenderer() {
    playerSprite = LoadTexture("assets/textures/player.png");
    grassSprite = LoadTexture("assets/textures/grass.png");
}

void deInitRenderer() {
    UnloadTexture(playerSprite);
    UnloadTexture(grassSprite);
}

void drawPlayer(Vector2 position) {
    DrawTexturePro(playerSprite, (Rectangle){ 0.0f, 0.0f, playerSprite.height, playerSprite.width }, 
        (Rectangle){ position.x, position.y, 32, 32 }, 
        (Vector2){0, 0}, 0.0f, WHITE);
}

void drawLevel(Level* level) {
    GameObject* roomObjects = level->objects;
    int tileSize = 20;
    for(int i = 0; i < level->objectCount; i++) {
        
        if(roomObjects[i].type == CHECKPOINT) {
            if(roomObjects[i].checkpoint.isActive)  
                DrawRectangle(roomObjects[i].position.x * tileSize, roomObjects[i].position.y * tileSize, tileSize, tileSize, GREEN);
            else 
                DrawRectangle(roomObjects[i].position.x * tileSize, roomObjects[i].position.y * tileSize, tileSize, tileSize, RED);
        }

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
}
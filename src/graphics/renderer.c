#include <stdio.h>
#include "renderer.h"
#include "config.h"

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

void drawPlayer(Vector2 position, int playerSize) {
    DrawTexturePro(playerSprite, 
        (Rectangle){ 0.0f, 0.0f, playerSprite.height, playerSprite.width }, 
        (Rectangle){ position.x, position.y, playerSize, playerSize }, 
        (Vector2){0, 0}, 0.0f, WHITE);
}

void drawBlock(Texture2D drawTexture, Vector2 position, int size) {
    DrawTexturePro(drawTexture, 
        (Rectangle){ 0.0f, 0.0f, drawTexture.height, drawTexture.width }, 
        (Rectangle){ position.x * tileSize, position.y * tileSize, size, size }, 
        (Vector2){0, 0}, 0.0f, WHITE);
}

void drawLevel(Level* level) {
    GameObject* roomObjects = level->objects;
    
    for(int i = 0; i < level->objectCount; i++) {
        switch(roomObjects[i].type) {
            case CHECKPOINT:
                DrawRectangle(roomObjects[i].position.x * tileSize, 
                    roomObjects[i].position.y * tileSize, tileSize, tileSize, 
                    roomObjects[i].checkpoint.isActive ? GREEN : RED);

                break;
            
            case OBJECT:
                switch (roomObjects[i].object.textureId) {
                    case GRASS:
                        drawBlock(grassSprite, roomObjects[i].position, roomObjects[i].object.size);
                        break;
                        
                    default:
                        drawBlock(grassSprite, roomObjects[i].position, roomObjects[i].object.size);
                        break;
                }
                break;

        default:
            break;
        }

    }
}
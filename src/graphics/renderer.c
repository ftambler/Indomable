#include <stdio.h>
#include "renderer.h"
#include "config.h"

Texture2D playerSprite;
Texture2D grassSprite;
Texture2D inactiveCheckpointSprite;
Texture2D activeCheckpointSprite;

static void drawObject(const GameObject gameObject, Texture2D drawTexture);
static void drawBlock(Texture2D drawTexture, Vector2 position, int scale);

void initRenderer() {
    playerSprite = LoadTexture("assets/textures/player.png");
    grassSprite = LoadTexture("assets/textures/grass.png");
    inactiveCheckpointSprite = LoadTexture("assets/textures/inactiveCheckpoint.png");
    activeCheckpointSprite = LoadTexture("assets/textures/activeCheckpoint.png");
}

void deInitRenderer() {
    UnloadTexture(playerSprite);
    UnloadTexture(grassSprite);
    UnloadTexture(inactiveCheckpointSprite);
    UnloadTexture(activeCheckpointSprite);
}

void drawPlayer(Vector2 position, int playerSize) {
    DrawTexturePro(playerSprite, 
        (Rectangle){ 0.0f, 0.0f, playerSprite.height, playerSprite.width }, 
        (Rectangle){ position.x, position.y, playerSize, playerSize }, 
        (Vector2){0, 0}, 0.0f, WHITE);
}

static void drawBlock(Texture2D drawTexture, Vector2 position, int scale) {
    DrawTexturePro(drawTexture, 
        (Rectangle){ 0.0f, 0.0f, drawTexture.height, drawTexture.width }, 
        (Rectangle){ position.x * tileSize, position.y * tileSize, tileSize * scale, tileSize * scale }, 
        (Vector2){0, 0}, 0.0f, WHITE);
}

static void drawObject(const GameObject gameObject, Texture2D drawTexture) {
    for(int i = 0; i < gameObject.object.size.x; i++) {
        for(int j = 0; j < gameObject.object.size.y; j++) {
            drawBlock(drawTexture, (Vector2){gameObject.position.x + i, gameObject.position.y + j}, gameObject.object.scale);
        }
    }
}

void drawLevel(Level* level) {
    GameObject* roomObjects = level->objects;
    
    for(int i = 0; i < level->objectCount; i++) {
        switch(roomObjects[i].type) {
            case CHECKPOINT:
                drawBlock(roomObjects[i].checkpoint.isActive ? activeCheckpointSprite : inactiveCheckpointSprite, roomObjects[i].position, 1);

                break;
            
            case OBJECT:
                Texture2D drawTexture;
                switch (roomObjects[i].object.textureId) {
                    case GRASS:
                    drawTexture = grassSprite;
                    break;
                    
                    default:
                    drawTexture = grassSprite;
                    break;
                }

                drawObject(roomObjects[i], drawTexture);
                break;

        default:
            break;
        }

    }
}
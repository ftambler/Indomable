#include <stdio.h>
#include "renderer.h"

Texture2D playerSprite;
// Texture2D grassSprite;

void initRenderer() {
    playerSprite = LoadTexture("assets/textures/player.png");
    // grassSprite = LoadTexture("assets/textures/grass.png");
}

void drawPlayer(Vector2 position) {

    DrawTexturePro(playerSprite, (Rectangle){ 0.0f, 0.0f, playerSprite.height, playerSprite.width }, 
        (Rectangle){ position.x, position.y, 32, 32 }, 
        (Vector2){0, 0}, 0.0f, WHITE);
}

void drawLevel(Level level) {

}
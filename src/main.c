#include <stdio.h>
#include "raylib.h"
#include "game.h"

// Screen Config
static const int screenWidth = 1200;
static const int screenHeight = 680;

// Funcs
static void UpdateDrawFrame(void);

int main(void) {
    InitWindow(screenWidth, screenHeight, "Juego Epicardo");
    SetTargetFPS(60);

    initGame();

    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }

    deInitGame();
    
    CloseWindow();
    return 0;
}

void UpdateDrawFrame(void) {
    updateGame(GetFrameTime());
    drawGame();
}
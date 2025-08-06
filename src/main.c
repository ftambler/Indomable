#include <stdio.h>

#include "raylib.h"

#include "screen.h"
#include "game.h"
#include "titleScreen.h"

#define screenWidth 1200
#define screenHeight 680

Screen currentScreen = TITLE_SCREEN;

int main(void) {
    InitWindow(screenWidth, screenHeight, "Juego Epicardo");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        switch (currentScreen) {
        case TITLE_SCREEN:
            updateTitleScreen();
            if(shouldStartGame()) {
                initGame();
                currentScreen = GAME_SCREEN;
            }
            break;
        
        case GAME_SCREEN:
            updateGameScreen();
            if(shouldStopGame()) {
                deInitGame();
                currentScreen = TITLE_SCREEN;
            }
            break;

        default:
            break;
        }

    }

    if(currentScreen == GAME_SCREEN)
        deInitGame();

    CloseWindow();
    return 0;
}
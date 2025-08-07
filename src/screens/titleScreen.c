#include "titleScreen.h"

#include "raylib.h"
#include "button.h"

bool startGameClicked = false;

Button startButton;

void initTitleScreen() {
    initButton(&startButton, (Rectangle){GetScreenWidth()/2 - 200/2, GetScreenHeight()/2 - 50/2, 200, 50}, "Start", GRAY, DARKGRAY, WHITE);
}

void updateTitleScreen() {
    updateTitle();
    drawTitle();
}

void updateTitle() {
    updateButton(&startButton);
    if (isButtonClicked(&startButton)) {
        startGameClicked = true;
    }
}

void drawTitle() {
    BeginDrawing();
    
    ClearBackground(RAYWHITE);

    DrawText("JUEGUITO", GetScreenWidth()/2 - MeasureText("JUEGUITO", 40)/2, GetScreenHeight()/2 - 120, 40, BLACK);
    drawButton(&startButton);

    EndDrawing();
}


bool shouldStartGame() {
    bool res = startGameClicked;
    startGameClicked = false;
    return res;
}
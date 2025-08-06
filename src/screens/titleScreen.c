#include "raylib.h"

#include "titleScreen.h"
#include "button.h"

bool startGameClicked = false;

Button startButton = {
    .bounds = { 100, 100, 200, 50 },
    .text = "Start",
    .bgColor = GRAY,
    .hoverColor = DARKGRAY,
    .textColor = WHITE,
};

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
    
    drawButton(&startButton);

    EndDrawing();
}


bool shouldStartGame() {
    bool res = startGameClicked;
    startGameClicked = false;
    return res;
}
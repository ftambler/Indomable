#include "pauseOverlay.h"

Button titleScreenButton = { 0 };
Button unpauseButton = { 0 };

void initPauseOverlay() {
    Rectangle pos = {(GetScreenWidth() / 2) - 100, (GetScreenHeight() / 2) - 40, 200, 80};
    initButton(&unpauseButton, pos, "Resume", GRAY, DARKGRAY, WHITE);
    pos.y += 120;
    initButton(&titleScreenButton, pos, "Exit to Menu", GRAY, DARKGRAY, WHITE);
}

void drawPauseOverlay() {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){50, 50, 50, 128});

    DrawText("Paused", GetScreenWidth()/2 - MeasureText("Paused", 40)/2, GetScreenHeight()/2 - 120, 40, BLACK);
    drawButton(&unpauseButton);
    drawButton(&titleScreenButton);
}

void updatePauseOverlay(bool* paused, bool* exitGameClicked) {
    updateButton(&titleScreenButton);
    updateButton(&unpauseButton);

    if(isButtonClicked(&titleScreenButton)) *exitGameClicked = true;
    if(isButtonClicked(&unpauseButton)) *paused = false;
}


#pragma once

#include <stdbool.h>

void updateGameScreen();

void initGame();
void deInitGame(void);

bool shouldStopGame();

void updateGame(float deltaTime);
void drawGame();
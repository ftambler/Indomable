#pragma once

#include "level.h"
#include "player.h"
#include "gameObject.h"

extern float gravity;
extern float groundFriction;

void updateGameScreen();

void initGame();
bool shouldStopGame();
void deInitGame(void);

void updateGame(float deltaTime);
void updatePauseMenu();

void drawGame();
void drawPausedMenuOverLay();
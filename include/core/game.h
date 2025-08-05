#pragma once

#include "level.h"
#include "player.h"
#include "gameObject.h"

extern float gravity;
extern float groundFriction;

void updateGameScreen();

void initGame();
void initPauseMenuButtons();
void deInitGame(void);

bool shouldStopGame();

void updateGame(float deltaTime);
void updatePauseMenu();

void drawGame();
void drawPausedMenuOverLay();
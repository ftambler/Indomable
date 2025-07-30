#pragma once

#include "level.h"
#include "player.h"
#include "gameObject.h"

// extern Level* levelArray;
// extern GameObject* roomObjects;
// extern int currentLevel;
// extern int level_count;
// extern Player player;

extern float gravity;
extern float groundFriction;

void updateGameScreen();

void initGame();
void deInitGame(void);

void updateGame(float deltaTime);
void drawGame();


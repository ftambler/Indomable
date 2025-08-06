#pragma once

#include "button.h"

extern Button titleScreenButton;
extern Button unpauseButton;

void initPauseOverlay();

void updatePauseOverlay(bool* paused, bool* exitGameClicked);

void drawPauseOverlay();
#pragma once

#include <stdbool.h>

#include "raylib.h"

typedef struct {
    Rectangle bounds;
    const char* text;
    Color bgColor;
    Color hoverColor;
    Color textColor;
    bool hovered;
    bool clicked;
} Button;

void updateButton(Button* button);
void drawButton(const Button* button);
bool isButtonClicked(const Button* button);
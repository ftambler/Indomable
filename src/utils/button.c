#include "button.h"

void initButton(Button* button, Rectangle bounds, char* text, Color bgColor, Color hoverColor, Color textColor) {
    button->bounds = bounds;
    button->text = text;
    button->bgColor = bgColor;
    button->hoverColor = hoverColor;
    button->textColor = textColor;
}

void updateButton(Button* button) {
    Vector2 mouse = GetMousePosition();
    button->hovered = CheckCollisionPointRec(mouse, button->bounds);
    button->clicked = button->hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void drawButton(const Button* button) {
    DrawRectangleRec(button->bounds, button->hovered ? button->hoverColor : button->bgColor);
    DrawText(button->text, button->bounds.x + button->bounds.width/2 - MeasureText(button->text, 20)/2, button->bounds.y + button->bounds.height/2 - 20/2, 20, button->textColor);
}

bool isButtonClicked(const Button* button) {
    return button->clicked;
}
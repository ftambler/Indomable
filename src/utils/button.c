#include "button.h"

void updateButton(Button* button) {
    Vector2 mouse = GetMousePosition();
    button->hovered = CheckCollisionPointRec(mouse, button->bounds);
    button->clicked = button->hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void drawButton(const Button* button) {
    DrawRectangleRec(button->bounds, button->hovered ? button->hoverColor : button->bgColor);
    DrawText(button->text, button->bounds.x + 10, button->bounds.y + 10, 20, button->textColor);
}

bool isButtonClicked(const Button* button) {
    return button->clicked;
}
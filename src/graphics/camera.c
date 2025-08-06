#include "camera.h"

#define DEADZONE_HEIGHT 150
#define DEADZONE_WIDTH 100

void initCamera(Camera2D* camera, const Vector2 playerPos) {
    camera->offset = (Vector2){ GetScreenWidth()/2, GetScreenHeight()/2 };
    camera->target = playerPos;
    camera->zoom = 1.0f;
}

void updateCamera(Camera2D* camera, const Vector2 playerPos) {
    Rectangle deadZone = {
        camera->target.x - DEADZONE_WIDTH / 2,
        camera->target.y - DEADZONE_HEIGHT / 2,
        DEADZONE_WIDTH,
        DEADZONE_HEIGHT
    };

    if (playerPos.x < deadZone.x) {
        camera->target.x = playerPos.x + DEADZONE_WIDTH / 2;
    } else if (playerPos.x > deadZone.x + deadZone.width) {
        camera->target.x = playerPos.x - DEADZONE_WIDTH / 2;
    }

    if (playerPos.y < deadZone.y) {
        camera->target.y = playerPos.y + DEADZONE_HEIGHT / 2;
    } else if (playerPos.y > deadZone.y + deadZone.height) {
        camera->target.y = playerPos.y - DEADZONE_HEIGHT / 2;
    }

    // Optionally clamp camera->target.x/y to world bounds

}
#include "physics.h"

float gravity = 500;
float groundFriction = 0.6f;

bool checkSqSqCollision(Vector2 obj1, int size1, Vector2 obj2, int size2) {
    return !(obj1.x > obj2.x * size2 + size2 ||
        obj1.x + size1 < obj2.x * size2 ||
        obj1.y > obj2.y * size2 + size2 ||
        obj1.y + size1 < obj2.y * size2
    );
}
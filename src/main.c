#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"

// Screen Config
static const int screenWidth = 1200;
static const int screenHeight = 680;

static const int tileSize = 20;

// Controls
static const int MOVE_LEFT = KEY_LEFT;
static const int MOVE_RIGHT = KEY_RIGHT;
static const int JUMP = KEY_UP;
static const int PAUSE = KEY_P;

// Player
typedef struct {
    Vector2 position;
    Vector2 velocity;
    int size;
    bool isGrounded;
    float jumpForce;
    float moveSpeed;
} Player;
static Player player;

Texture2D playerSprite;

typedef struct Object {
    Vector2 position;
    bool hasCollision;
    int textureId;
    int size;
} Object;
const int totalObjects = 1;
Object gameObjects[1];


// Physics
static float gravity = 10;
static float groundFriction = -0.6f;

// Funcs
static void UpdateDrawFrame(void);
static void UpdateGame(float deltaTime);
static void DrawGame(void);
static void InitGame(void);
static void DeInitialize(void);

static bool checkCollision(Vector2 obj1, int size1, Vector2 obj2, int size2);
static void handlePlayerCollision(Vector2 *playerPos, Vector2 *playerVel, int playerSize, bool *grounded, Vector2 obj, int objSize);

//DEBUG
static void printPlayerPosition(void);
static void printPlayerVelocity(void);

int main(void) {
    InitWindow(screenWidth, screenHeight, "Juego Epicardo");
    SetTargetFPS(60);

    InitGame();

    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }

    DeInitialize();
    
    CloseWindow();
    return 0;
}

void InitGame() {
    player.position = (Vector2){screenWidth/2, screenHeight/2};
    player.velocity = (Vector2){0, 0};
    
    player.size = 20;
    player.isGrounded = true;
    player.moveSpeed = 5;
    player.jumpForce = 5;
    
    playerSprite = LoadTexture("assets/textures/player.png");

    gameObjects[0].position.x = 5;
    gameObjects[0].position.y = 31;
    gameObjects[0].size = 20;
}
void DeInitialize() {
    UnloadTexture(playerSprite);
}

void UpdateDrawFrame(void) {
    UpdateGame(GetFrameTime());
    DrawGame();
}

void UpdateGame(float deltaTime) {
    // Input
    if (IsKeyDown(MOVE_RIGHT)) player.velocity.x += player.moveSpeed;
    if (IsKeyDown(MOVE_LEFT))  player.velocity.x -= player.moveSpeed;
    if (IsKeyPressed(JUMP) && player.isGrounded) {
        player.velocity.y = -player.jumpForce;
        player.isGrounded = false;
    }

    // Gravity
    player.velocity.y += gravity * deltaTime;

    // Position
    player.position.x += player.velocity.x + deltaTime;
    player.position.y += player.velocity.y + deltaTime;

    // Collision (ground)
    if (player.position.y + player.size > screenHeight) {
        player.position.y = screenHeight - player.size;
        player.velocity.y = 0;
        player.isGrounded = true;
    }
    for(int i = 0; i < totalObjects; i++) {
        if(checkCollision(player.position, player.size, gameObjects[i].position, gameObjects[i].size)) {
            handlePlayerCollision(&player.position, &player.velocity, player.size, &player.isGrounded, gameObjects[i].position, tileSize);
        }
    }

    // Friction
    player.velocity.x *= groundFriction;
}

bool checkCollision(Vector2 obj1, int size1, Vector2 obj2, int size2) {
    return !(obj1.x > obj2.x * tileSize + size2 ||
        obj1.x + size1 < obj2.x * tileSize ||
        obj1.y > obj2.y * tileSize + size2 ||
        obj1.y + size1 < obj2.y * tileSize
    );
}

void handlePlayerCollision(Vector2 *playerPos, Vector2 *playerVel, int playerSize, bool *grounded, Vector2 obj, int objSize) {
    // Compute overlaps
    float overlapX = (playerPos->x + playerSize) - (obj.x * objSize);
    float overlapX2 = ((obj.x * objSize) + objSize) - playerPos->x;
    float overlapY = (playerPos->y + playerSize) - (obj.y * objSize);
    float overlapY2 = ((obj.y * objSize) + objSize) - playerPos->y;

    // Determine minimal overlap on each axis
    float minX = (overlapX < overlapX2) ? overlapX : -overlapX2;
    float minY = (overlapY < overlapY2) ? overlapY : -overlapY2;

    // Choose the axis with shallower push-out
    if (fabsf(minX) < fabsf(minY)) {
        // Horizontal collision
        playerPos->x -= minX;
        playerVel->x = 0;
    } else {
        // Vertical collision
        playerPos->y -= minY;
        playerVel->y = 0;
        if (minY > 0) {
            *grounded = true; // landed on top
        }
    }
}


void printPlayerPosition(void) {
    printf("x: %f, y: %f \n", player.position.x, player.position.y);
}

void printPlayerVelocity(void) {
    printf("x: %f, y: %f \n", player.velocity.x, player.velocity.y);
}

void DrawGame(void) {
    BeginDrawing();
    
    ClearBackground(RAYWHITE);
    
    for(int i = 0; i < 1; i++) {
        DrawRectangle(gameObjects[i].position.x*tileSize, gameObjects[i].position.y*tileSize, tileSize, tileSize, BLACK);
    }

    DrawTexture(playerSprite, player.position.x, player.position.y, WHITE);

    EndDrawing();
}
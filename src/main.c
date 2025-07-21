#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "raylib.h"
#include "cJSON.h"

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

typedef struct {
    bool hasCollision;
    int textureId;
    int size;
} Object;

typedef enum { OBJECT } Type;
typedef struct {
    Vector2 position;
    Type type;
    union {
        Object object;
    };
} GameObject;

typedef struct {
    int objectCount;
    GameObject* objects;
} Level;
Level* levelArray;
GameObject* roomObjects;
int currentLevel = 0;

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

static int getEnumOfValue(char* value);

//DEBUG
static void printPlayerPosition(void);
static void printPlayerVelocity(void);

int loadLevel() {
    FILE *fp = fopen("assets/levels/level1.json", "r");
    if (fp == NULL) {
        printf("Error: Unable to open the file.\n");
        return 1;
    }

    // read the file contents into a string
    char buffer[1024];
    int len = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    cJSON* root = cJSON_Parse(buffer);
    cJSON* levels = cJSON_GetObjectItem(root, "Level");
    int level_count = cJSON_GetArraySize(levels);

    levelArray = malloc(level_count * sizeof(Level));

    for (int i = 0; i < level_count; i++) {
        cJSON* level_entry = cJSON_GetArrayItem(levels, i);
        cJSON* level_key = level_entry->child;
        cJSON* obj_array = cJSON_GetObjectItem(level_entry, level_key->string);

        int objCount = cJSON_GetArraySize(obj_array);
        levelArray[i].objectCount = objCount;
        levelArray[i].objects = malloc(objCount * sizeof(GameObject));

        for (int j = 0; j < objCount; j++) {
            cJSON* obj = cJSON_GetArrayItem(obj_array, j);
            cJSON* type = cJSON_GetObjectItem(obj, "type");
            cJSON* pos = cJSON_GetObjectItem(obj, "position");

            levelArray[i].objects[j].type = getEnumOfValue(type->valuestring);

            levelArray[i].objects[j].position.x = cJSON_GetObjectItem(pos, "x")->valueint;
            levelArray[i].objects[j].position.y = cJSON_GetObjectItem(pos, "y")->valueint;
        }
    }
}

int getEnumOfValue(char* value) {
    if(value == "OBJECT") return 0;
    return 0;
}

int main(void) {
    InitWindow(screenWidth, screenHeight, "Juego Epicardo");
    SetTargetFPS(60);

    loadLevel();

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

    roomObjects = levelArray[0].objects;
}
void DeInitialize() {
    UnloadTexture(playerSprite);
    free(levelArray);
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
    for(int i = 0; i < levelArray[currentLevel].objectCount; i++) {
        if(checkCollision(player.position, player.size, roomObjects[i].position, tileSize)) {
            handlePlayerCollision(&player.position, &player.velocity, player.size, &player.isGrounded, roomObjects[i].position, tileSize);
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

void DrawGame(void) {
    BeginDrawing();
    
    ClearBackground(RAYWHITE);
    
    for(int i = 0; i < levelArray->objectCount; i++) {
        DrawRectangle(roomObjects[i].position.x * tileSize, roomObjects[i].position.y * tileSize, tileSize, tileSize, BLACK);
    }

    DrawTexture(playerSprite, player.position.x, player.position.y, WHITE);

    EndDrawing();
}


void printPlayerPosition(void) {
    printf("x: %f, y: %f \n", player.position.x, player.position.y);
}

void printPlayerVelocity(void) {
    printf("x: %f, y: %f \n", player.velocity.x, player.velocity.y);
}
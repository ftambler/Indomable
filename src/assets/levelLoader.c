#include <stdio.h>
#include <stdlib.h>

#include "levelLoader.h"

#include "cJSON.h"
#include "level.h"
#include "config.h"

// TODO HANDLE ERRORS

static char* readFileToBuffer(const char* path);
static Level parseLevelFromJSON(cJSON* level_entry, GameObject** activeCheckpoint);
static void parseGameObject(cJSON* obj, GameObject* out, GameObject** activeCheckpoint);

static int getIntOrDefault(cJSON* obj, const char* key, int defaultVal);

int loadLevel(int* level_count, Level** levelArray, GameObject** activeCheckpoint) {
    char* buffer = readFileToBuffer("assets/levels/level1.json");
    if (!buffer) return 1;

    cJSON* root = cJSON_Parse(buffer);
    free(buffer);
    if (!root) return 1;

    cJSON* levels = cJSON_GetObjectItem(root, "Level");
    *level_count = cJSON_GetArraySize(levels);
    *levelArray = malloc((*level_count) * sizeof(Level));
    if (!*levelArray) {
        cJSON_Delete(root);
        return 1;
    }

    for (int i = 0; i < (*level_count); i++) {
        cJSON* level_entry = cJSON_GetArrayItem(levels, i);
        (*levelArray)[i] = parseLevelFromJSON(level_entry, activeCheckpoint);
    }

    cJSON_Delete(root);
    return 0;
}

static char* readFileToBuffer(const char* path) {
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Error: Unable to open the file.\n");
        return NULL;
    }

    // Dynamically read file
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    rewind(fp);

    char *buffer = malloc(fileSize + 1);
    if (!buffer) {
        fclose(fp);
        printf("Error: Failed to allocate buffer.\n");
        return NULL;
    }

    fread(buffer, 1, fileSize, fp);
    buffer[fileSize] = '\0';
    fclose(fp);
    return buffer;
}

static Level parseLevelFromJSON(cJSON* level_entry, GameObject** activeCheckpoint) {
    Level level = {0};

    cJSON* level_key = level_entry->child;
    cJSON* obj_array = cJSON_GetObjectItem(level_entry, level_key->string);

    int objCount = cJSON_GetArraySize(obj_array);
    level.objectCount = objCount;
    level.objects = malloc(objCount * sizeof(GameObject));

    for (int j = 0; j < objCount; j++) {
        cJSON* obj = cJSON_GetArrayItem(obj_array, j);
        parseGameObject(obj, &level.objects[j], activeCheckpoint);
    }

    return level;
}

static void parseGameObject(cJSON* obj, GameObject* out, GameObject** activeCheckpoint) {
    cJSON* type = cJSON_GetObjectItem(obj, "type");
    cJSON* pos = cJSON_GetObjectItem(obj, "position");

    out->position.x = cJSON_GetObjectItem(pos, "x")->valueint;
    out->position.y = cJSON_GetObjectItem(pos, "y")->valueint;

    switch(getEnumOfType(type->valuestring)) {
        case OBJECT:
            out->type = OBJECT;
            out->object.size = getIntOrDefault(obj, "scale", 1) * tileSize;
            out->object.textureId = GRASS;
            break;

        case SPAWN:
            out->type = CHECKPOINT;
            out->checkpoint.isActive = true;
            *activeCheckpoint = out;
            break;

        case CHECKPOINT:
            out->type = CHECKPOINT;
            out->checkpoint.isActive = false;
            break;

        default:
            break;
    }
}

int getIntOrDefault(cJSON* obj, const char* key, int defaultVal) {
    cJSON* item = cJSON_GetObjectItemCaseSensitive(obj, key);
    return cJSON_IsNumber(item) ? item->valueint : defaultVal;
}
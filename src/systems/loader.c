#include <stdio.h>
#include <stdlib.h>

#include "loader.h"

#include "cJSON.h"
#include "level.h"

int loadLevel(int* level_count, Level** levelArray, GameObject** activeCheckpoint) {
    FILE *fp = fopen("assets/levels/level1.json", "r");
    if (fp == NULL) {
        printf("Error: Unable to open the file.\n");
        return 1;
    }

    // Dynamically read file
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    rewind(fp);

    char *buffer = malloc(fileSize + 1);
    if (!buffer) {
        fclose(fp);
        printf("Error: Failed to allocate buffer.\n");
        return 1;
    }

    fread(buffer, 1, fileSize, fp);
    buffer[fileSize] = '\0';
    fclose(fp);

    cJSON* root = cJSON_Parse(buffer);
    free(buffer);
    if (!root) {
        printf("Error: Failed to parse JSON.\n");
        return 1;
    }

    cJSON* levels = cJSON_GetObjectItem(root, "Level");
    *level_count = cJSON_GetArraySize(levels);
    *levelArray = malloc((*level_count) * sizeof(Level));
    if (!*levelArray) {
        printf("Error: Failed to allocate levels.\n");
        cJSON_Delete(root);
        return 1;
    }

    Level* levelsPtr = *levelArray;

    for (int i = 0; i < (*level_count); i++) {
        cJSON* level_entry = cJSON_GetArrayItem(levels, i);
        cJSON* level_key = level_entry->child;
        cJSON* obj_array = cJSON_GetObjectItem(level_entry, level_key->string);

        int objCount = cJSON_GetArraySize(obj_array);
        levelsPtr[i].objectCount = objCount;
        levelsPtr[i].objects = malloc(objCount * sizeof(GameObject));

        for (int j = 0; j < objCount; j++) {
            cJSON* obj = cJSON_GetArrayItem(obj_array, j);
            cJSON* type = cJSON_GetObjectItem(obj, "type");
            cJSON* pos = cJSON_GetObjectItem(obj, "position");

            levelsPtr[i].objects[j].position.x = cJSON_GetObjectItem(pos, "x")->valueint;
            levelsPtr[i].objects[j].position.y = cJSON_GetObjectItem(pos, "y")->valueint;
            
            // GameObject Type
            switch(getEnumOfType(type->valuestring)) {
                case OBJECT:
                    levelsPtr[i].objects[j].type = OBJECT;
                    // TODO ASSIGN TEXTURE
                    levelsPtr[i].objects[j].object.textureId = GRASS;
                    break;
                    
                case SPAWN:
                    levelsPtr[i].objects[j].type = CHECKPOINT;
                    levelsPtr[i].objects[j].checkpoint.isActive = true;
                    *activeCheckpoint = &levelsPtr[i].objects[j];
                    break;

                case CHECKPOINT:
                    levelsPtr[i].objects[j].type = CHECKPOINT;
                    levelsPtr[i].objects[j].checkpoint.isActive = false;
                    break;    
                
                default:
                    break;
            }

        }
    }

    cJSON_Delete(root);
    return 0;
}

#pragma once

#include <stdbool.h>

typedef struct {
    bool isActive;
} Checkpoint;

void setCheckpointActive(Checkpoint* checkpoint);

void setCheckpointInactive(Checkpoint* checkpoint);

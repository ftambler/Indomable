#include <stdio.h>
#include "checkpoint.h"

void setCheckpointActive(Checkpoint* checkpoint) {
    checkpoint->isActive = true;
}

void setCheckpointInactive(Checkpoint* checkpoint) {
    checkpoint->isActive = false;
}


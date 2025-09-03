#pragma once

#include <pthread.h>
#include "framebuffer.h"
#include "configreader.h"

#define MAX_CMD_LENGTH 500

typedef struct pThreadArgs {
    FrameBuffer *fb;
    Sources *s;
    enum MType type;
} pThreadArgs;

void * init_ff_process(void * args);
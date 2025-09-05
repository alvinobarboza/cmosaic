#pragma once

#include <pthread.h>
#include "framebuffer.h"
#include "configreader.h"

#define MAX_RES_LENGTH 12
#define MAX_CMD_LENGTH 500

typedef struct pThreadArgs {
    FrameBuffer *fb;
    Sources *s;
    enum MType type;
    uint16_t screen_width;
    uint16_t screen_height;
    bool close;
} pThreadArgs;

void * init_ff_process(void * args);
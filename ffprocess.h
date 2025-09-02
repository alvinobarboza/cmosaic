#pragma once

#include <pthread.h>
#include "framebuffer.h"
#include "configreader.h"

typedef struct pThreadArgs {
    FrameBuffer *fb;
    Sources *s;
    enum MType type;
} pThreadArgs;

void * init_ff_process(void * args);
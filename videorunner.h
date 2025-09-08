#pragma once

#include "framequeue.h"
#include "framebuffer.h"
#include "configreader.h"
#include "videodata.h"
#include "ffprocess.h"


typedef struct RunningVideo
{
    FrameQueue *fq;
    FrameBuffer *fb;
    VideoData *vd;
    pthread_t thread;
    pThreadArgs *arg;
} RunningVideo;

RunningVideo task[MAX_SOURCES];

bool videorunner_init(ConfigFile *config, uint16_t w , uint16_t h );

void videorunner_free();
#pragma once

#include "configreader.h"
#include "framequeue.h"

typedef struct VideoData
{
    Sources *source;
    FrameQueue *queue;
    uint32_t frame_size;
    uint8_t *current_frame;
} VideoData;

VideoData *videdata_new(Sources *s, FrameQueue *fq, uint32_t frame_size);
void videodata_free(VideoData *vd);
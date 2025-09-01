#pragma once

#include "configreader.h"
#include "framequeue.h"

#include "include/raylib/include/raylib.h"

#define COLORS_CHANNEL 3

typedef struct Vec2
{
    uint32_t x, y;
} Vec2;


typedef struct VideoData
{
    Sources *source;
    FrameQueue *queue;
    Vec2 resolution;
    Vec2 posisition;
    uint8_t *current_frame_buf;
} VideoData;

VideoData *videdata_new(Sources *s, FrameQueue *fq, Vec2 res, Vec2 pos);
void videodata_free(VideoData *vd);

void videodata_update_canvas(VideoData *vd, Color *canvas, uint32_t w, uint32_t h);
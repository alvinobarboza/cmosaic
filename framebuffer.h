#pragma once
#include "framequeue.h"

typedef struct FrameBuffer {
    uint8_t     *readBuf;
    uint32_t    frame_size;
    uint32_t    color_index;
    FrameQueue  *queue;
} FrameBuffer;

FrameBuffer *framebuffer_new(uint32_t frame_size, FrameQueue *queue);
void framebuffer_free(FrameBuffer *fb);

void framebuffer_write_data(FrameBuffer *fb, uint8_t color);
#pragma once

#include <stdlib.h>
#include <stdint.h>

#define MAX_FRAME_QUEUE 15
#define MIN_FRAME_QUEUE 3

typedef struct FrameQueue {
    uint8_t capacity;
    uint8_t head;
    uint8_t tail;
    uint8_t size;
    uint8_t **queue;
    uint32_t frame_size;
} FrameQueue;

FrameQueue *framequeue_new(uint8_t buffer_size, uint32_t frame_size);
void framequeue_free(FrameQueue *fm);
void framequeue_enqueue(FrameQueue *fm, uint8_t *data);
uint8_t *framequeue_dequeue(FrameQueue *fm);

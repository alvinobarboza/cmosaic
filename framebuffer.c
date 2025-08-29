#include "framebuffer.h"

FrameBuffer *framebuffer_new(uint32_t frame_size, FrameQueue *queue) {
    FrameBuffer *fb = malloc(sizeof(FrameBuffer));
    fb->color_index = 0;
    fb->frame_size = frame_size;
    fb->queue = queue;
    fb->readBuf = malloc(sizeof(uint8_t)*frame_size);
    return fb;
}

void framebuffer_free(FrameBuffer *fb) {
    free(fb->readBuf);
    free(fb);
}

void framebuffer_write_data(FrameBuffer *fb, uint8_t color) {
    fb->readBuf[fb->color_index] = color;
    fb->color_index++;

    if (fb->color_index == fb->frame_size) {
        framequeue_enqueue(fb->queue, fb->readBuf);
        fb->color_index = 0;
    }
}

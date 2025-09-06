#include "framebuffer.h"
#include <sys/time.h>

struct timeval tval_before, tval_after, tval_result;

FrameBuffer *framebuffer_new(uint32_t frame_size, FrameQueue *queue) {
    FrameBuffer *fb = malloc(sizeof(FrameBuffer));
    if (fb == NULL) {
        return NULL;
    }

    fb->color_index = 0;
    fb->frame_size = frame_size;
    fb->queue = queue;
    fb->readBuf = malloc(sizeof(uint8_t)*frame_size);
    
    if (fb->readBuf == NULL) {
        free(fb);
        return NULL;
    }

    gettimeofday(&tval_before, NULL);
    return fb;
}

void framebuffer_free(FrameBuffer *fb) {
    if (fb != NULL){
        free(fb->readBuf);
        free(fb);
    }
}

void framebuffer_write_data(FrameBuffer *fb, uint8_t color) {
    fb->readBuf[fb->color_index] = color;
    fb->color_index++;
    
    if (fb->color_index == fb->frame_size) {
        gettimeofday(&tval_after, NULL);
        
        timersub(&tval_after, &tval_before, &tval_result);
        
        printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);

        gettimeofday(&tval_before, NULL);
        framequeue_enqueue(fb->queue, fb->readBuf);
        fb->color_index = 0;
    }
}

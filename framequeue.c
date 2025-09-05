#include "framequeue.h"
#include <stdio.h>

FrameQueue *framequeue_new(uint8_t buffer_size, uint32_t frame_size) {
    buffer_size = buffer_size < MIN_FRAME_QUEUE ? MIN_FRAME_QUEUE : buffer_size;
    buffer_size = buffer_size > MAX_FRAME_QUEUE ? MAX_FRAME_QUEUE : buffer_size;

    FrameQueue *fq = malloc(sizeof(FrameQueue));
    if (fq == NULL){
        return NULL;
    }

    fq->capacity = buffer_size;
    fq->head = 0;
    fq->tail = 0;
    fq->size = 0;
    fq->frame_size = frame_size;

    fq->queue = malloc(sizeof(uint8_t*)*buffer_size);
    if (fq->queue == NULL) {
        free(fq);
        return NULL;
    }

    for (uint32_t i = 0; i < buffer_size; i++)
    {
        fq->queue[i] = malloc(sizeof(uint8_t)*frame_size);
        if (fq->queue[i] == NULL) {
            for (uint32_t j = 0; j < i; j++){
                free(fq->queue[j]);
            }
            free(fq->queue);
            free(fq);
            return NULL;
        }
    }

    if (pthread_mutex_init(&fq->lock,NULL) != 0) {
        for (uint32_t i = 0; i < buffer_size; i++)
        {
            free(fq->queue[i]);
        }
        free(fq->queue);
        free(fq);
        return NULL;
    }

    return fq;
}

void framequeue_free(FrameQueue *fq) {
    if (fq != NULL) 
    {
        for (int i = 0; i < fq->capacity; i++)
        {
            free(fq->queue[i]);
        }
        pthread_mutex_destroy(&fq->lock);
        free(fq->queue);
        free(fq);
    }
}

void framequeue_enqueue(FrameQueue *fq, uint8_t *data) {
    pthread_mutex_lock(&fq->lock);

    memcpy(fq->queue[fq->tail], data, fq->frame_size);

    fq->tail = (fq->tail + 1) % fq->capacity;

    if (fq->size == fq->capacity)
    {
        fq->head = (fq->head + 1) % fq->capacity;
        pthread_mutex_unlock(&fq->lock);
        return;
    }

    fq->size += 1;

    pthread_mutex_unlock(&fq->lock);
}

bool framequeue_dequeue(FrameQueue *fq, uint8_t *data) {
    pthread_mutex_lock(&fq->lock);
    if (fq->size == 0)
    {
        pthread_mutex_unlock(&fq->lock);
        return false;
    }

    memcpy(data, fq->queue[fq->head], fq->frame_size);

    fq->head = (fq->head + 1) % fq->capacity;
    fq->size -= 1;

    pthread_mutex_unlock(&fq->lock);

    return true;
}
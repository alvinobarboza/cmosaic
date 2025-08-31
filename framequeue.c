#include "framequeue.h"

FrameQueue *framequeue_new(uint8_t buffer_size, uint32_t frame_size) {
    buffer_size = buffer_size < MIN_FRAME_QUEUE ? MIN_FRAME_QUEUE : buffer_size;
    buffer_size = buffer_size > MAX_FRAME_QUEUE ? MAX_FRAME_QUEUE : buffer_size;

    FrameQueue *fq = malloc(sizeof(FrameQueue));
    fq->capacity = buffer_size;
    fq->head = 0;
    fq->tail = 0;
    fq->size = 0;
    fq->frame_size = frame_size;

    fq->queue = malloc(sizeof(uint8_t)*buffer_size);

    for (int i = 0; i < fq->capacity; i++)
    {
        fq->queue[i] = malloc(sizeof(uint8_t)*fq->frame_size);
    }

    pthread_mutex_init(&fq->lock, NULL);

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
        return false;
    }

    memcpy(data, fq->queue[fq->head], fq->frame_size);

    fq->head = (fq->head + 1) % fq->capacity;
    fq->size -= 1;

    pthread_mutex_unlock(&fq->lock);

    return true;
}
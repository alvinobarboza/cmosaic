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

FrameQueue *framequeue_new(uint8_t buffer_size, uint32_t frame_size) {
    buffer_size = buffer_size < MIN_FRAME_QUEUE ? MIN_FRAME_QUEUE : buffer_size;

    FrameQueue *fm = malloc(sizeof(FrameQueue));
    fm->capacity = buffer_size;
    fm->head = 0;
    fm->tail = 0;
    fm->size = 0;
    fm->frame_size = frame_size;

    fm->queue = malloc(sizeof(uint8_t)*buffer_size);

    for (int i = 0; i < fm->capacity; i++)
    {
        fm->queue[i] = malloc(sizeof(uint8_t)*fm->frame_size);
    }

    return fm;
}

void framequeue_free(FrameQueue *fm) {
    if (fm != NULL) 
    {
        for (int i = 0; i < fm->capacity; i++)
        {
            free(fm->queue[i]);
        }
        free(fm->queue);
        free(fm);
    }
}

void framequeue_enqueue(FrameQueue *fm, uint8_t *data) {
    memcpy(fm->queue[fm->tail], data, fm->frame_size);

    fm->tail = (fm->tail + 1) % fm->capacity;

    if (fm->size == fm->capacity)
    {
        fm->head = (fm->head + 1) % fm->capacity;
        return;
    }

    fm->size += 1;
}

uint8_t *framequeue_dequeue(FrameQueue *fm) {
    if (fm->size == 0)
    {
        return NULL;
    }

    uint8_t *data = malloc(sizeof(uint8_t)*fm->frame_size);
    memcpy(data, fm->queue[fm->head], fm->frame_size);

    fm->head = (fm->head + 1) % fm->capacity;
    fm->size -= 1;

    return data;
}
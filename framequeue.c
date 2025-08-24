#include <inttypes.h>

#define MAX_FRAME_BUFFER 15

typedef struct framequeue
{
    uint8_t capacity;
    uint8_t head;
    uint8_t tail;
    uint8_t size;
    uint8_t *queue[MAX_FRAME_BUFFER];
} Framequeue;

Framequeue new_framequeue(){

}
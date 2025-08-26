#include <stdio.h>
#include <time.h>
#include "framequeue.h"

int main(void) {
    srand(time(NULL));

    uint32_t frame_size = 1920*1080;

    FrameQueue *fm = framequeue_new(3, frame_size);
    uint8_t *temp_frame = malloc(sizeof(uint8_t) * frame_size);
    uint8_t *frame_data = malloc(sizeof(uint8_t) * frame_size);

    for (int i = 0; i < 10000; i++)
    {
        for(int j = 0; j < frame_size; j++){
            int r = rand() % 250;
            temp_frame[j] = r;
        }

        framequeue_enqueue(fm, temp_frame);

        if (i % 2 == 0) {
            ;
            if(framequeue_dequeue(fm, frame_data)){
                for(int j = 0; j < frame_size; j++){
                    printf("rgb: %03d \r",frame_data[j]);
                }
            }
        }
    }
    
    free(temp_frame);
    free(frame_data);
    framequeue_free(fm);
    puts("");
    return 0;
}
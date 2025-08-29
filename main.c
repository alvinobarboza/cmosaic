#include <stdio.h>
#include <time.h>
#include "framequeue.h"
#include "framebuffer.h"
#include "configreader.h"

int main(void) {
    srand(time(NULL));

    ConfigFile *cf = read_config();

    printf("%d \n\n",cf->type);

    free(cf);

    uint32_t frame_size = 1920*1080;

    FrameQueue *fq = framequeue_new(3, frame_size);
    FrameBuffer *fb = framebuffer_new(frame_size, fq);
    uint8_t *frame_data = malloc(sizeof(uint8_t) * frame_size);

    for (uint32_t i = 0; i < 1; i++)
    {
        for(uint32_t j = 0; j < frame_size; j++){
            uint8_t r = rand() % 250;
            framebuffer_write_data(fb, r);
        }

        if (i % 2 == 0) {
            if(framequeue_dequeue(fq, frame_data)){
                for(uint32_t j = 0; j < frame_size; j++){
                    printf("rgb: %03d \r",frame_data[j]);
                }
            }
        }
    }
    
    free(frame_data);
    framequeue_free(fq);
    framebuffer_free(fb);
    puts("");
    return 0;
}
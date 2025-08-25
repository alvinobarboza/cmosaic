int main(void) {
    uint8_t frame_size = 4;

    FrameQueue *fm = framequeue_new(3, frame_size);
    uint8_t *temp_frame = malloc(sizeof(uint8_t) * frame_size);
    
    for (int i = 0; i < 50; i++)
    {
        for(int j = 0; j < frame_size; j++){
            int r = rand() % 250;
            temp_frame[j] = r;
        }

        framequeue_enqueue(fm, temp_frame);

        if (i % 2 == 0) {
            uint8_t * data = framequeue_dequeue(fm);
            if(data != NULL){
                puts("\n");
                for(int j = 0; j < frame_size; j++){
                    if (j % 10 == 0) {
                        printf("%02d\n", j);
                    }
                    printf("p: %03d ",data[j]);
                }
                free(data);
            }
        }
    }

    framequeue_free(fm);
    return 0;
}
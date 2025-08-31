#include "videodata.h"

VideoData *videdata_new(Sources *s, FrameQueue *fq, uint32_t frame_size) {
    VideoData *vd = malloc(sizeof(VideoData));
    vd->queue = fq;
    vd->source = s;
    vd->frame_size = frame_size;

    vd->current_frame = malloc(sizeof(uint8_t) * frame_size);

    return vd;
}
 
void videodata_free(VideoData *vd) {
    if (vd != NULL) {
        free(vd->current_frame);
        free(vd);
    }
}
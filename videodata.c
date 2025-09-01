#include "videodata.h"

VideoData *videdata_new(Sources *s, FrameQueue *fq,  Vec2 res, Vec2 pos) {
    VideoData *vd = malloc(sizeof(VideoData));
    vd->queue = fq;
    vd->source = s;
    vd->posisition = pos;
    vd->resolution = res;

    vd->current_frame_buf = malloc(sizeof(uint8_t) * res.x * res.y * COLORS_CHANNEL);

    return vd;
}
 
void videodata_free(VideoData *vd) {
    if (vd != NULL) {
        free(vd->current_frame_buf);
        free(vd);
    }
}


void videodata_update_canvas(VideoData *vd, Color *canvas, uint32_t w, uint32_t h) {
    if (framequeue_dequeue(vd->queue, vd->current_frame_buf)) {
        for (uint32_t y = 0; y < vd->resolution.y; y++) 
        {
            for (uint32_t x = 0; x < vd->resolution.x*COLORS_CHANNEL; x+=3) 
            {
                uint32_t index = (y+vd->posisition.y)*w+(x+vd->posisition.x);
                if (index >= w*h*COLORS_CHANNEL) {
                    continue;
                }
                canvas[index/COLORS_CHANNEL].r = vd->current_frame_buf[index+0];
                canvas[index/COLORS_CHANNEL].g = vd->current_frame_buf[index+1];
                canvas[index/COLORS_CHANNEL].b = vd->current_frame_buf[index+2];
                canvas[index/COLORS_CHANNEL].a = 255;
            }
        }        
    }
}
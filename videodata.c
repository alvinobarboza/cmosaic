#include "videodata.h"

VideoData *videodata_new(Sources *s, FrameQueue *fq,  Vec2 res, Vec2 pos) {
    VideoData *vd = malloc(sizeof(VideoData));
    if (vd == NULL){
        return NULL;
    }

    vd->queue = fq;
    vd->source = s;
    vd->position = pos;
    vd->resolution = res;

    vd->current_frame_buf = malloc(sizeof(uint8_t) * res.x * res.y * COLORS_CHANNEL);
    if (vd->current_frame_buf == NULL){
        free(vd);
        return NULL;
    }

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
                uint32_t i_video = y*vd->resolution.x*COLORS_CHANNEL+x;
                uint32_t i_canvas = (y + vd->position.y) * w + x / COLORS_CHANNEL + vd->position.x;

                if (i_canvas >= w*h*COLORS_CHANNEL || i_video+2 >= vd->queue->frame_size) {
                    continue;
                }
                canvas[i_canvas].r = vd->current_frame_buf[i_video+0];
                canvas[i_canvas].g = vd->current_frame_buf[i_video+1];
                canvas[i_canvas].b = vd->current_frame_buf[i_video+2];
                canvas[i_canvas].a = 255;
            }
        }        
    }
}
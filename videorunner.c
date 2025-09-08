#include "videorunner.h"

bool is_loaded(ConfigFile *config, uint16_t index)  
{
    bool loaded = false;
    for (uint16_t i = 0; i < 10; i++){
        if (config->sources[index].name[i] != NOT_LOADED[i]) {
            loaded = true;
        }
    }
    return loaded;
}

Vec2 position_index(enum MType t, uint16_t index,  uint16_t w , uint16_t h ) {
    Vec2 v = (Vec2){0};

    uint8_t i = 0;
    for (uint8_t y = 0; y < t; y++) {
        for (uint8_t x = 0; x < t; x++) {
            if (i == index) {
                v.x = w/t * x;
                v.y = h/t * y;
                return v;
            }
            i++;
        }   
    }
    return v;
}

bool videorunner_init( ConfigFile *config, uint16_t w , uint16_t h ) {

    uint16_t video_count = 0;

    for (uint16_t i = 0; i < MAX_SOURCES; i++)
    {
        if (is_loaded(config, i)) {
            task[i].arg = malloc(sizeof(pThreadArgs));
            
            uint32_t frame_size = (w/config->type)*(h/config->type)*COLORS_CHANNEL;
            task[i].fq = framequeue_new(MAX_FRAME_QUEUE, frame_size);
            if (task[i].fq == NULL){
                free(task[i].arg);
                task[i].arg = NULL;
                videorunner_free();
                
                perror("Failed to init frame queue");
                return false;
            }

            task[i].fb = framebuffer_new(frame_size, task[i].fq);
            if (task[i].fb == NULL){
                free(task[i].arg);
                free(task[i].fq);
                task[i].arg = NULL;
                videorunner_free();
                
                perror("Failed to init frame buffer");
                return false;
            }

            task[i].vd = videodata_new(
                &config->sources[i], 
                task[i].fq, 
                (Vec2){x:w/config->type,y:h/config->type},
                position_index(config->type, i, w, h)
            );
            if (task[i].vd == NULL) {
                free(task[i].arg);
                free(task[i].fq);
                free(task[i].fb);
                task[i].arg = NULL;
                videorunner_free();
                
                perror("Failed to init video data");
                return false;
            }

            task[i].arg->fb = task[i].fb;
            task[i].arg->s = &config->sources[i];
            task[i].arg->type = config->type;
            task[i].arg->screen_height = h;
            task[i].arg->screen_width = w;
            task[i].arg->close = false;

            int err = pthread_create(&task[i].thread, NULL, init_ff_process, (void *)task[i].arg);
            if (err != 0) {
                free(task[i].arg);
                free(task[i].fq);
                free(task[i].fb);
                free(task[i].vd);
                task[i].arg = NULL;

                videorunner_free();
                perror("Failed to start thread");
                return false;
            }
            
            video_count++;
            continue;
        } 

        task[i].arg = NULL;
    }

    if (video_count == 0) return false;

    return true;
}

void videorunner_free(){
    for (uint16_t i = 0; i < MAX_SOURCES; i++)
    {
        if (task[i].arg != NULL) {
            free(task[i].arg);
            videodata_free(task[i].vd);
            framebuffer_free(task[i].fb);
            framequeue_free(task[i].fq);
        }
    }
}
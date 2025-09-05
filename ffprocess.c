#include "ffprocess.h"

#include <stdio.h>
#include <stdint.h>

#include "framebuffer.h"

void resolution(char res[MAX_RES_LENGTH], enum MType t, uint16_t w, uint16_t h) {
    snprintf(res, 12, "%d:%d", w/t, h/t);
}

void make_cmd(char *cmd, const char *input, const char *resolution) {
    snprintf(cmd, MAX_CMD_LENGTH, "ffmpeg -hide_banner -re -stream_loop -1 -i '%s' -filter_complex '[0:v:0]scale=%s:flags=fast_bilinear,fps=fps=15,format=rgb24[vi]' -map '[vi]' -f rawvideo pipe:1", input, resolution);
}

void * init_ff_process(void * arguments) {
    pThreadArgs *args = (pThreadArgs *) arguments;

    FILE *fp;
    int16_t charout;

    char command[MAX_CMD_LENGTH];
    char res[MAX_RES_LENGTH];
    resolution(res, args->type, args->screen_width, args->screen_height);

    make_cmd(command, args->s->source, res);

    fp = popen(command, "r");
    if (fp==NULL){
        perror("Failed to start ffmpeg process!");
        pthread_exit(NULL);
        return 0;
    }

    while((charout=fgetc(fp))!=EOF ) {
        framebuffer_write_data(args->fb, charout);
        if (args->close) break;
    }

    pclose(fp);
    pthread_exit(NULL);
    return 0;
}

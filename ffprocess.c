#include "ffprocess.h"

#include <stdio.h>
#include <stdint.h>

#include "framebuffer.h"

char *resolution(enum MType t) {
    switch (t)
    {
    case T_1X1:
        return "1920:1080";
    case T_2X2:
        return "960:540";
    case T_3X3:
        return "640:360";
    default:
        return "\0";
    } 
}

void make_cmd(char *cmd, const char *input, const char *resolution) {
    snprintf(cmd, MAX_CMD_LENGTH, "ffmpeg -hide_banner -re -i '%s' -filter_complex '[0:v:0]scale=%s:flags=fast_bilinear,fps=fps=15,format=rgb24[vi]' -map '[vi]' -f rawvideo pipe:1", input, resolution);
}

void * init_ff_process(void * arguments) {
    pThreadArgs args = *(pThreadArgs *) arguments;

    FILE *fp;
    int16_t charout;

    char command[MAX_CMD_LENGTH];
    char *res = resolution(args.type);
    make_cmd(command, args.s->source, res);

    fp = popen(command, "r");
    if (fp==NULL){
        puts("Failed to start ffmpeg process!");
        pthread_exit(NULL);
        return 0;
    }

    while((charout=fgetc(fp))!=EOF) {
        framebuffer_write_data(args.fb, charout);
    }

    pclose(fp);
    pthread_exit(NULL);
    return 0;
}

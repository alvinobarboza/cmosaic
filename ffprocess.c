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

void * init_ff_process(void * arguments) {
    pThreadArgs args = *(pThreadArgs *) arguments;

    FILE *fp;
    uint8_t charout;

    char *res = resolution(args.type);

    fp = popen("ffmpeg -hide_banner -re -i 'udp:234.50.99.2:6000?overrun_nonfatal=1&fifo_size=1000000&localaddr=10.50.0.160' -filter_complex '[0:v:0]scale=120:30:flags=fast_bilinear,fps=fps=15,format=rgb24[vi]' -map '[vi]' -f rawvideo pipe:1", "r");
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

#include <stdio.h>
#include <time.h>

#include "framequeue.h"
#include "framebuffer.h"
#include "configreader.h"
#include "videodata.h"
#include "ffprocess.h"
#include "include/raylib/include/raylib.h"

#define WIDTH 1920
#define HEIGHT 1080

int main(void) {
    ConfigFile *confg = read_config();
    if (confg->err) {
        printf("err: %d", confg->err);
        return 1;
    }

    for (uint8_t i = 0; i < confg->type*confg->type; i++){
        printf("input: %s\n", confg->sources[i].source);
    }

    uint32_t frame_size = (WIDTH/confg->type)*(HEIGHT/confg->type)*COLORS_CHANNEL;

    FrameQueue *fq = framequeue_new(MAX_FRAME_QUEUE, frame_size);
    if (fq == NULL){
        perror("Failed to init frame queue");
        return 1;
    }

    FrameBuffer *fb = framebuffer_new(frame_size, fq);
     if (fb == NULL){
        framequeue_free(fq);
        perror("Failed to init frame buffer");
        return 1;
    }

    VideoData *vd = videodata_new(
        &confg->sources[0], 
        fq, 
        (Vec2){x:WIDTH/confg->type,y:HEIGHT/confg->type},
        (Vec2){0}
    );
    if (vd == NULL) {
        framebuffer_free(fb);
        framequeue_free(fq);
        perror("Failed to init video data");
        return 1;
    }

    pThreadArgs args = {
        .fb = fb,
        .s = &confg->sources[0],
        .type = confg->type,
        .screen_height = HEIGHT,
        .screen_width = WIDTH,
        .close = false,
    };

    pthread_t thread1;
    pthread_create(&thread1, NULL, init_ff_process, (void *)&args);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_WINDOW_ALWAYS_RUN);
    InitWindow(WIDTH, HEIGHT, "Mosaic Window");
    
    RenderTexture2D render_tex = LoadRenderTexture(WIDTH, HEIGHT);
    Image img = GenImageColor(WIDTH, HEIGHT, BLACK);
    Texture2D texture = LoadTextureFromImage(img);

    UnloadImage(img);

    Color pixels[WIDTH*HEIGHT];

    uint16_t square_height = HEIGHT / 2;
    uint16_t square_width = WIDTH / 2;

    uint16_t off_x = square_width / 2;
    uint16_t off_y = square_height / 2;

    for (uint32_t i = 0; i < WIDTH*HEIGHT; i++) {
        pixels[i].r = 10;
        pixels[i].g = 50;
        pixels[i].b = 100;
        pixels[i].a = 255;
    }

    for (uint16_t y = 0; y < square_height ; y++) {
        for (uint16_t x = 0; x < square_width; x++) {
            uint32_t i = (y+off_y)*WIDTH+(x+off_x);
            pixels[i].r = (((x+off_x) * 100)/square_width) % 255;
            pixels[i].g = (((y+off_y) * 100)/square_height) % 255;
            pixels[i].b = 0;
            pixels[i].a = 255;
        }
    }

    UpdateTexture(texture, pixels);
    SetTargetFPS(90);

    float tw = (float)WIDTH;
    float th = (float)HEIGHT;

    float hRatio = th / tw;
    float wRatio = tw / th;

    Vector2 origin = {
        x: 0.0f,
        y: 0.0f,
    };
    
    uint8_t fps_counter = 0;
    while (!WindowShouldClose()) {
        BeginTextureMode(render_tex);
            ClearBackground(BLACK);
            DrawTexture(texture,0,0,WHITE);
        EndTextureMode();

        if (IsWindowResized()) {
            float w = (float) GetScreenWidth();
            float h = (float) GetScreenHeight();
            printf("cur w=%02f h=%02f\n", w,h);

            if ((w/h) > wRatio) {
                tw = h * wRatio;
                th = h;
                origin.x = (tw - w) / 2;
                origin.y = 0;
            }

            if ((h/w) > hRatio) {
                th = w * hRatio;
                tw = w;
                origin.x = 0;
                origin.y = (th - h) / 2;
            }

            printf("new w=%02f h=%02f\n", tw,th);
        }
            
        BeginDrawing();
            ClearBackground(BLACK);

            DrawTexturePro(
                render_tex.texture, 
                (Rectangle){x:0, y:0, width: (float)WIDTH, height: -(float)HEIGHT}, 
                (Rectangle){x:0, y:0, width: tw, height: th},
                origin,
                0.0f,
                WHITE
            );

            DrawFPS(10, 10);
        EndDrawing();

        if (fps_counter % 3 == 0) {
            videodata_update_canvas(vd, pixels, WIDTH, HEIGHT);
            UpdateTexture(texture, pixels);
        }

        fps_counter++;
    }
    
    args.close = true;

    pthread_join(thread1, NULL);

    free(confg);
    videodata_free(vd);
    framebuffer_free(fb);
    framequeue_free(fq);
    UnloadTexture(texture);
    UnloadRenderTexture(render_tex);
    CloseWindow();
    return 0;
}
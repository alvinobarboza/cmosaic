#include <stdio.h>
#include <time.h>

#include "framequeue.h"
#include "framebuffer.h"
#include "configreader.h"
#include "videodata.h"
#include "ffprocess.h"
#include "videorunner.h"
#include "raylib.h"

#define WIDTH 1920
#define HEIGHT 1080

int main(void) {
    ConfigFile *confg = read_config();
    if (confg->err) {
        print_error_hint(confg->err);
        return 1;
    }
    
    for (uint8_t i = 0; i < confg->type*confg->type; i++){
        printf("input: %s\n", confg->sources[i].source);
    }
    
    if (!videorunner_init(confg, WIDTH, HEIGHT)){
        free(confg);
        return 1;
    }

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_WINDOW_ALWAYS_RUN);
    InitWindow(WIDTH, HEIGHT, "Mosaic Window");
    
    RenderTexture2D render_tex = LoadRenderTexture(WIDTH, HEIGHT);
    Image img = GenImageColor(WIDTH, HEIGHT, BLACK);
    Texture2D texture = LoadTextureFromImage(img);

    UnloadImage(img);

    Color pixels[WIDTH*HEIGHT];

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
            for (size_t i = 0; i < MAX_SOURCES; i++) {
                if (task[i].arg != NULL){
                    videodata_update_canvas(task[i].vd, pixels, WIDTH, HEIGHT);
                }
            }
            UpdateTexture(texture, pixels);
        }

        fps_counter++;
    }

    for (size_t i = 0; i < MAX_SOURCES; i++) {
        if (task[i].arg != NULL) {
            task[i].arg->close = true;
        }
    }  

    for (size_t i = 0; i < MAX_SOURCES; i++) {
        if (task[i].arg != NULL) {
            pthread_join(task[i].thread, NULL);
        }
    }

    free(confg);
    
    UnloadTexture(texture);
    UnloadRenderTexture(render_tex);
    CloseWindow();
    return 0;
}
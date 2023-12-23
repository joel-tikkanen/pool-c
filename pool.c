#include "pool.h"
#include "include/raylib.h"

int main(void){

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "pool");
    SetTargetFPS(10);


    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();
    }

     
    CloseWindow();

    return 0;


}
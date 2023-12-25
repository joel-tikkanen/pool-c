#include "pool.h"
#include "include/raylib.h"


void init_ball(int num, enum GameState gs) {
    return;
}
void init_balls() {
    return;
}

void update_ball(Ball *ball) {
    return;
}

void handle_ball_collision(Ball *ball1, Ball *ball2) {
    return;
}

void handle_wall_collision(Ball *ball) {

}
void check_collisions() {

}
void check_pockets() {

}
void handle_pocket(Ball *pocketed) {

}
void render_ball(Ball *ball) {

}
void render_stick(Stick *stick) {

};


void init(){ 
    // game initialization to starting point
    return;
}



int main(void){

    enum GameState game_state = NOT_HIT;
    enum Type first_collision = NONE;

    

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "pool");
    SetTargetFPS(10);


    while (!WindowShouldClose()) {

        switch (game_state) {
            case NOT_HIT:
                // stick input
                // hit ball input
                // change state when ball hit
                // set first_collision to NONE
                break;
            case HIT:
                // set first collision if collision happens
                // update ball positions
                // check and handle collisions
                // change state to NOT_HIT if no handball
                // change state to HANDBALL if handball
            case HANDBALL:
                // move ball input
                // change state when ball moved
                break;
        }
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();
    }

     
    CloseWindow();

    return 0;


}
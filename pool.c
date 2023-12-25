#include "pool.h"
#include "include/raylib.h"
#include <math.h>



Ball init_ball(int num, enum Type type, float x, float y) {
    Ball b = {0};

    b.num = num;
    b.type = type;
    b.pocketed = false;
    b.x = x;
    b.y = y;
    b.vx = 100;

    return b;
}
void init_balls(Ball (*balls)[BALL_COUNT]) {
    (*balls)[0] = init_ball(0, SOLID, 500, 100);
    (*balls)[1] = init_ball(0, SOLID, 100, 90);
    (*balls)[0].vx = -100;
    return;
}


void update_balls(Ball (*balls)[BALL_COUNT]) {

    for (int i = 0; i < BALL_COUNT; i++){
        (*balls)[i].x += (*balls)[i].vx*DT;
        (*balls)[i].y += (*balls)[i].vy*DT;
    }
    return;
}


float sq_distance(float x1, float y1, float x2, float y2) {
    return (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);
}


float distance(float x1, float y1, float x2, float y2) {
    return (float)sqrt(sq_distance(x1, y1, x2, y2));
}

void handle_ball_collision(Ball *ball1, Ball *ball2) {
    float x1, x2, y1, y2;
    x1 = ball1->x;
    x2 = ball2->x;
    y1 = ball1->y;
    y2 = ball2->y;

    float sq_dist = sq_distance(x1, y1, x2, y2);
    float dist = distance(x1, y1, x2, y2);

    float diff_x = x2 - x1;
    float diff_y = y2 - y1;

    float normal_x = diff_x / dist;
    float normal_y = diff_y / dist;


    // separating balls
    ball1->x += normal_x * BALL_RADIUS;
    ball2->x -= normal_x * BALL_RADIUS;
    ball1->y += normal_y * BALL_RADIUS;
    ball2->y -= normal_y * BALL_RADIUS;


    // after separation
    x1 = ball1->x;
    x2 = ball2->x;
    y1 = ball1->y;
    y2 = ball2->y;

    normal_x = diff_x / dist;
    normal_y = diff_y / dist;

    float vx1 = ball1->vx;
    float vy1 = ball1->vy;
    float vx2 = ball2->vx;
    float vy2 = ball2->vy;

    // elastic response
    float p = (vx1*normal_x+vy1*normal_y) - (vx2*normal_x+vy2*normal_y);

    float new_vx_1 = vx1 - p * normal_x;
    float new_vy_1 = vy1 - p * normal_y;

    float new_vx_2 = vx2 + p * normal_x;
    float new_vy_2 = vy2 + p * normal_y;

    if (new_vx_1 < SCREEN_WIDTH && new_vy_1 < SCREEN_WIDTH && new_vx_2 < SCREEN_WIDTH && new_vy_2 < SCREEN_WIDTH) {
        ball1->vx = new_vx_1;
        ball1->vy = new_vy_1;
        ball2->vx = new_vx_2;
        ball2->vy = new_vy_2;
    }
    return;
}

void check_collisions(Ball (*balls)[BALL_COUNT]) {
    for (int i = 0; i < BALL_COUNT; i++){
        if ((*balls)[i].x + BALL_RADIUS >= SCREEN_WIDTH || (*balls)[i].x - BALL_RADIUS <= 0) {
            (*balls)[i].vx *= -1;
        }

        if ((*balls)[i].y + BALL_RADIUS >= SCREEN_HEIGHT || (*balls)[i].y - BALL_RADIUS <= 0) {
            (*balls)[i].vy *= -1;
        }
        for (int j = i+1; j < BALL_COUNT; j++){
            float x1, x2, y1, y2;
            x1 = (*balls)[i].x;
            x2 = (*balls)[j].x;
            y1 = (*balls)[i].y;
            y2 = (*balls)[j].y;
            if (sq_distance(x1,y1, x2, y2) <= (BALL_RADIUS*BALL_RADIUS)){
                printf("collsion!");
                handle_ball_collision(&(*balls)[i], &(*balls)[j]);
            }
        }
    }

}

void check_pockets(Ball (*balls)[BALL_COUNT]) {

}

void handle_pocket(Ball *pocketed) {

}

void render_balls(Ball (*balls)[BALL_COUNT]) {
    for (int i = 0; i < BALL_COUNT; i++){
        DrawCircle((*balls)[i].x, (*balls)[i].y, BALL_RADIUS, WHITE);
    }
}



void render_stick(Stick *stick) {

};


int main(void){

    enum GameState game_state = NOT_HIT;
    enum Type first_collision = NONE;

    Ball (*balls)[BALL_COUNT] = malloc(sizeof(Ball[BALL_COUNT]));

    init_balls(balls);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "pool");
    SetTargetFPS(FPS);

    while (!WindowShouldClose()) {
        
        
       
        switch (game_state) {
            case NOT_HIT:
                update_balls(balls);
                check_collisions(balls);
                
                
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
                break;
            case HANDBALL:
                // move ball input
                // change state when ball moved
                break;
        }
        BeginDrawing();
        // render board
        // render balls
        render_balls(balls);

        // render stick
        ClearBackground(BLACK);
        EndDrawing();
    }

    free(balls);
    CloseWindow();

    return 0;


}
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
    b.vx = 0;

    return b;
}
void init_balls(Ball (*balls)[BALL_COUNT]) {
    (*balls)[0] = init_ball(0, SOLID, 500, 100);
    (*balls)[1] = init_ball(0, SOLID, 100, 90);
    (*balls)[0].vx = -150;
    (*balls)[1].vx = 150;
    return;
}


void update_balls(Ball (*balls)[BALL_COUNT], float dt) {
    // only balls not pocketed
    for (int i = 0; i < BALL_COUNT; i++){
        (*balls)[i].x += (*balls)[i].vx * dt;
        (*balls)[i].y += (*balls)[i].vy * dt;
        (*balls)[i].vx += (*balls)[i].ax * dt;
        (*balls)[i].vy += (*balls)[i].ay * dt;
        (*balls)[i].ax = 0.5 * -((*balls)[i].vx);
        (*balls)[i].ay = 0.5 * -((*balls)[i].vy);
        if (fabs((*balls)[i].vx) <= 0.1 && fabs((*balls)[i].ax) <= 0.1) (*balls)[i].vx = 0.0;
        if (fabs((*balls)[i].vy) <= 0.1 && fabs((*balls)[i].ay) <= 0.1) (*balls)[i].vy = 0.0;
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

    float offset =  (BALL_DIAMETER - dist) * 0.6;

    // separating balls
    ball1->x += normal_x * offset;
    ball2->x -= normal_x * offset;
    ball1->y += normal_y * offset;
    ball2->y -= normal_y * offset;


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

void check_collisions(Ball (*balls)[BALL_COUNT], enum Type *fc) {
    // only balls not pocketed
    for (int i = 0; i < BALL_COUNT; i++){
        if ((*balls)[i].pocketed) continue;
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
            if (sq_distance(x1,y1, x2, y2) <= (BALL_DIAMETER*BALL_DIAMETER)){
                if ((*balls)[i].num == 0 && *fc == NONE) {
                        *fc = (*balls)[j].type;
                }
                handle_ball_collision(&(*balls)[i], &(*balls)[j]);
            }
        }
    }
    return;

}

void check_pockets(Ball (*balls)[BALL_COUNT]) {
    // check balls distance to pockets.
    // TODO: define pocket positions
    // if ball pocketed, handle pocket
    return;
}

void handle_pocket(enum Type turn, Ball *pocketed, enum GameState *gs) {
    // if turn is NONE then set turn to pocketed ball type
    // change pocketed ball pocketed attribute
    // if turn is not NONE and own ball pocketed then set game state to HANDBALL
    // check win/lose conditions
    // set turn to opposite color

    // white ball to center
    // dont change white balls attribute pocketed
    return;
}

void render_balls(Ball (*balls)[BALL_COUNT]) {
     // only balls not pocketed
    for (int i = 0; i < BALL_COUNT; i++){
        DrawCircle((*balls)[i].x, (*balls)[i].y, BALL_RADIUS, WHITE);
    }
    return;
}

bool is_moving(Ball (*balls)[BALL_COUNT]){
    for (int i = 0; i < BALL_COUNT; i++){
        if (fabs((*balls)[i].vx) >= 0.1 || fabs((*balls)[i].vy) >= 0.1) return true;
    }
    return false;
}





void render_stick(Stick *stick) {
    if (stick->hide) return;

    struct Vector2 start;
    struct Vector2 end;

    start.x = stick->start_x;
    start.y = stick->start_y;
    end.x = stick->end_x;
    end.y = stick->end_y;

    
    DrawLineEx(start, end, 4.0, WHITE);
   
    return;
};

void set_stick(Stick* stick, float mouse_x, float mouse_y, float wb_x, float wb_y){


    float dist = distance(mouse_x, mouse_y, wb_x, wb_y);

    float diff_x = mouse_x - wb_x;
    float diff_y = mouse_y - wb_y;

    if (dist <= BALL_RADIUS) {
        // select angle to hit ball
        stick->normal_x = diff_x / dist;
        stick->normal_y = diff_y / dist;
        stick->start_x = wb_x + stick->normal_x * BALL_RADIUS; 
        stick->start_y = wb_y + stick->normal_y * BALL_RADIUS; 
        stick->end_x = wb_x - stick->normal_x * -STICK_LENGTH;
        stick->end_y = wb_y - stick->normal_y * -STICK_LENGTH;
    } else {
        // select power of the hit
        stick->start_x = wb_x + stick->normal_x * BALL_RADIUS + stick->normal_x * dist; 
        stick->start_y = wb_y + stick->normal_y * BALL_RADIUS + stick->normal_y * dist; 
        stick->end_x = wb_x - stick->normal_x * -STICK_LENGTH + stick->normal_x * dist;
        stick->end_y = wb_y - stick->normal_y * -STICK_LENGTH + stick->normal_y * dist;
    }
    
    return;
}

int main(void){

    enum GameState game_state = HIT;
    enum Type first_collision = NONE;
    enum Type turn = NONE;

    float dt = 1.0/FPS;




    Ball (*balls)[BALL_COUNT] = malloc(sizeof(Ball[BALL_COUNT]));
    Stick stick = {0};

    struct Vector2 mouse;


    init_balls(balls);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "pool");
    SetTargetFPS(FPS);

    while (!WindowShouldClose()) {
        
        
       
        switch (game_state) {
            case NOT_HIT:                
                // stick input
                stick.hide = true;        
                if (IsMouseButtonDown(0)) {
                    stick.hide = false;
                    mouse = GetMousePosition();
                    set_stick(&stick, mouse.x,  mouse.y, (*balls)[0].x, (*balls)[0].y);
                }
                
                // hit ball input
                // change state when ball hit

                // set first_collision to NONE
                first_collision = NONE;
                break;
            case HIT:
                // set first collision if collision happens
                // update ball positions
                // check and handle collisions
                // TODO: generalized function to position checking
                
                check_collisions(balls, &first_collision);
                check_pockets(balls);
                update_balls(balls, dt);
                // change state to NOT_HIT if no handball
                // change state to HANDBALL if handball
                if (!is_moving(balls)) {
                    printf("not hit!!");
                    if ((first_collision != turn && turn != NONE) || first_collision == NONE) {
                        game_state = HANDBALL;
                    } else {
                        game_state = NOT_HIT;

                    }
                } 
                break;
            case HANDBALL:
                // move ball input
                // change state when ball moved
                break;
            case WIN_SOLID:
                // restart
                // display result, restart btn, stats, etc
                break;
            case WIN_STRIPE:
                // restart
                // display result, restart btn, stats, etc
                break;
        }
        BeginDrawing();
        // render board
        // render balls
        render_balls(balls);
        render_stick(&stick);

        // render stick
        ClearBackground(BLACK);
        EndDrawing();

        dt = GetFrameTime(); 
    }

    free(balls);
    CloseWindow();

    return 0;


}
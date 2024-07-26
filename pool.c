#include "pool.h"
#include "include/raylib.h"
#include <math.h>

Ball init_ball(int num, enum Type type, float x, float y)
{
    Ball b = {0};

    b.num = num;
    b.type = type;
    b.pocketed = false;
    b.x = x;
    b.y = y;
    b.vx = 0;

    return b;
}
void init_balls(Ball (*balls)[BALL_COUNT])
{
    (*balls)[0] = init_ball(0, NONE, 375, 300); 
    (*balls)[1] = init_ball(1, SOLID, 375, 100);
    (*balls)[2] = init_ball(2, SOLID, 395, 80);
    (*balls)[3] = init_ball(3, SOLID, 355, 80);
    (*balls)[4] = init_ball(4, SOLID, 415, 60);
    (*balls)[5] = init_ball(5, SOLID, 375, 60);
    (*balls)[6] = init_ball(6, SOLID, 335, 60);
    (*balls)[7] = init_ball(7, SOLID, 435, 40);
    (*balls)[8] = init_ball(8, EIGHT, 375, 40);
    (*balls)[9] = init_ball(9, STRIPES, 315, 60);
    (*balls)[10] = init_ball(10, STRIPES, 395, 40);
    (*balls)[11] = init_ball(11, STRIPES, 355, 40);
    (*balls)[12] = init_ball(12, STRIPES, 455, 20);
    (*balls)[13] = init_ball(13, STRIPES, 415, 20);
    (*balls)[14] = init_ball(14, STRIPES, 375, 20);
    (*balls)[15] = init_ball(15, STRIPES, 335, 20);
    return;
}

void update_balls(Ball (*balls)[BALL_COUNT], float dt)
{
    // only balls not pocketed
    for (int i = 0; i < BALL_COUNT; i++)
    {
        (*balls)[i].x += (*balls)[i].vx * dt;
        (*balls)[i].y += (*balls)[i].vy * dt;
        (*balls)[i].vx += (*balls)[i].ax * dt;
        (*balls)[i].vy += (*balls)[i].ay * dt;
        (*balls)[i].ax = FRICTION * -((*balls)[i].vx);
        (*balls)[i].ay = FRICTION * -((*balls)[i].vy);
        if (fabs((*balls)[i].vx) <= 0.1 && fabs((*balls)[i].ax) <= 0.1)
            (*balls)[i].vx = 0.0;
        if (fabs((*balls)[i].vy) <= 0.1 && fabs((*balls)[i].ay) <= 0.1)
            (*balls)[i].vy = 0.0;
    }
    return;
}

float sq_distance(float x1, float y1, float x2, float y2)
{
    return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}

float distance(float x1, float y1, float x2, float y2)
{
    return (float)sqrt(sq_distance(x1, y1, x2, y2));
}

bool too_fast(float vx, float vy)
{
    return false;
}

void handle_ball_collision(Ball *ball1, Ball *ball2)
{
    float x1, x2, y1, y2;
    x1 = ball1->x;
    x2 = ball2->x;
    y1 = ball1->y;
    y2 = ball2->y;

    float sq_dist = sq_distance(x1, y1, x2, y2);
    float dist = sqrt(sq_dist);

    float diff_x = x2 - x1;
    float diff_y = y2 - y1;

    float normal_x = diff_x / dist;
    float normal_y = diff_y / dist;

    // float offset =  (BALL_DIAMETER - dist) * 0.6;

    // if (dist > 0.0) {
    //     normal_x = diff_x / dist;
    //     normal_y = diff_y / dist;

    //     ball1->x += normal_x * offset;
    //     ball2->x -= normal_x * offset;
    //     ball1->y += normal_y * offset;
    //     ball2->y -= normal_y * offset;
    // }

    // after separation
    // x1 = ball1->x;
    // x2 = ball2->x;
    // y1 = ball1->y;
    // y2 = ball2->y;

    // diff_x = x2 - x1;
    // diff_y = y2 - y1;

    // dist = distance(x1, y1, x2, y2);

    // normal_x = diff_x / dist;
    // normal_y = diff_y / dist;

    // elastic response
    float p = (ball1->vx * normal_x + ball1->vy * normal_y) - (ball2->vx * normal_x + ball2->vy * normal_y);

    float new_vx_1 = ball1->vx - p * normal_x;
    float new_vy_1 = ball1->vy - p * normal_y;

    float new_vx_2 = ball2->vx + p * normal_x;
    float new_vy_2 = ball2->vy + p * normal_y;

    if (new_vx_1 * (1 / FPS) < SCREEN_WIDTH && new_vy_1 * (1 / FPS) < SCREEN_HEIGHT && new_vx_2 * (1 / FPS) < SCREEN_WIDTH && new_vy_2 * (1 / FPS) < SCREEN_HEIGHT)
    {
        ball1->vx = new_vx_1;
        ball1->vy = new_vy_1;
        ball2->vx = new_vx_2;
        ball2->vy = new_vy_2;
    }
    return;
}

void check_collisions(Ball (*balls)[BALL_COUNT], enum Type *fc)
{
    // only balls not pocketed
    for (int i = 0; i < BALL_COUNT; i++)
    {
        if ((*balls)[i].pocketed)
            continue;
        if ((*balls)[i].x + BALL_RADIUS >= SCREEN_WIDTH - WALL_PADDING || (*balls)[i].x - BALL_RADIUS <= WALL_PADDING)
        {
            (*balls)[i].vx *= -1;
        }

        if ((*balls)[i].y + BALL_RADIUS >= SCREEN_HEIGHT - WALL_PADDING || (*balls)[i].y - BALL_RADIUS <= WALL_PADDING)
        {
            (*balls)[i].vy *= -1;
        }
        for (int j = i + 1; j < BALL_COUNT; j++)
        {
            float x1, x2, y1, y2;
            x1 = (*balls)[i].x;
            x2 = (*balls)[j].x;
            y1 = (*balls)[i].y;
            y2 = (*balls)[j].y;
            if (sq_distance(x1, y1, x2, y2) <= (BALL_DIAMETER * BALL_DIAMETER))
            {
                if ((*balls)[i].num == 0 && *fc == NONE)
                {
                    *fc = (*balls)[j].type;
                }
                handle_ball_collision(&(*balls)[i], &(*balls)[j]);
            }
        }
    }
    return;
}

void check_pockets(Ball (*balls)[BALL_COUNT], enum GameState gs, enum Type turn)
{
    for (int i = 0; i < BALL_COUNT; ++i)
    {
        if ((*balls)[i].pocketed)
            continue;

        for (int j = 0; j < 6; ++j)
        {
            float dx = (*balls)[i].x - pockets[j].x;
            float dy = (*balls)[i].y - pockets[j].y;
            float dist_squared = dx * dx + dy * dy;

            if (dist_squared <= BALL_RADIUS * BALL_RADIUS)
            {
                handle_pocket(turn, &(*balls)[i], gs);
                break;
            }
        }
    }
}

void handle_pocket(enum Type turn, Ball *pocketed, enum GameState gs)
{
    if (pocketed->type == NONE)
    {
        pocketed->x = 375;
        pocketed->y = 200;
        pocketed->pocketed = false;
    }
    else
    {

        pocketed->pocketed = true;

        if (turn == NONE)
        {

            turn = pocketed->type;
        }
        else if (turn != pocketed->type)
        {
            gs = HANDBALL;
            turn = turn == SOLID ? STRIPES : SOLID;
        }
    }
}

void draw_balls(Ball (*balls)[BALL_COUNT])
{
    // only balls not pocketed
    for (int i = 0; i < BALL_COUNT; i++)
    {
        DrawCircle((*balls)[i].x, (*balls)[i].y, BALL_RADIUS, WHITE);
    }

    return;
}

void draw_table(Texture2D table_texture)
{
    DrawTexture(table_texture, SCREEN_WIDTH / 2 - table_texture.width / 2, SCREEN_HEIGHT / 2 - table_texture.height / 2 + 35, WHITE);
}

bool is_moving(Ball (*balls)[BALL_COUNT])
{
    for (int i = 0; i < BALL_COUNT; i++)
    {
        if (fabs((*balls)[i].vx) >= 0.1 || fabs((*balls)[i].vy) >= 0.1)
            return true;
    }
    return false;
}

bool in_bounds(int x, int y)
{
    if ((x + BALL_RADIUS >= SCREEN_WIDTH - WALL_PADDING || x - BALL_RADIUS <= WALL_PADDING) || (y + BALL_RADIUS >= SCREEN_HEIGHT - WALL_PADDING || y - BALL_RADIUS <= WALL_PADDING))
    {
        return false;
    }
    return true;
}

void hit_ball(Ball *white_ball, float force_x, float force_y)
{
    printf("ball hit");
    white_ball->vx = force_x * 0.05;
    white_ball->vy = force_y * 0.05;
    return;
}

void draw_stick(Stick *stick)
{
    if (stick->hide)
        return;

    struct Vector2 start;
    struct Vector2 end;

    start.x = stick->start_x;
    start.y = stick->start_y;
    end.x = stick->end_x;
    end.y = stick->end_y;

    DrawLineEx(start, end, 4.0, WHITE);

    return;
};

void set_stick(Stick *stick, float mouse_x, float mouse_y, float wb_x, float wb_y)
{

    float dist = distance(mouse_x, mouse_y, wb_x, wb_y);

    float diff_x = mouse_x - wb_x;
    float diff_y = mouse_y - wb_y;

    if (dist <= BALL_RADIUS)
    {
        // set angle of the shot
        stick->normal_x = diff_x / dist;
        stick->normal_y = diff_y / dist;
        stick->start_x = wb_x + stick->normal_x * BALL_RADIUS;
        stick->start_y = wb_y + stick->normal_y * BALL_RADIUS;
        stick->end_x = wb_x - stick->normal_x * -STICK_LENGTH;
        stick->end_y = wb_y - stick->normal_y * -STICK_LENGTH;
    }
    else
    {
        // set power of the shot
        stick->start_x = wb_x + stick->normal_x * BALL_RADIUS + stick->normal_x * dist;
        stick->start_y = wb_y + stick->normal_y * BALL_RADIUS + stick->normal_y * dist;
        stick->end_x = wb_x - stick->normal_x * -STICK_LENGTH + stick->normal_x * dist;
        stick->end_y = wb_y - stick->normal_y * -STICK_LENGTH + stick->normal_y * dist;
    }

    return;
}

int main(void)
{

    enum GameState game_state = HIT;
    enum Type first_collision = NONE;
    enum Type turn = NONE;

    float dt = 1.0 / FPS;

    Ball(*balls)[BALL_COUNT] = malloc(sizeof(Ball[BALL_COUNT]));
    Stick stick = {0};

    struct Vector2 mouse;

    init_balls(balls);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "pool");

    Image img = LoadImage("Pool_Table_Type_1_BG Wide.png");
    ImageResize(&img, SCREEN_WIDTH + 75, SCREEN_HEIGHT + 200);

    Texture2D texture = LoadTextureFromImage(img);

    UnloadImage(img);

    SetTargetFPS(80);

    while (!WindowShouldClose())
    {
        mouse = GetMousePosition();
        switch (game_state)
        {

        case NOT_HIT:
            // stick input
            stick.hide = true;

            if (IsMouseButtonDown(0))
            {
                stick.hide = false;

                set_stick(&stick, mouse.x, mouse.y, (*balls)[0].x, (*balls)[0].y);
            }

            float sq_dist = sq_distance(mouse.x, mouse.y, (*balls)[0].x, (*balls)[0].y);
            if (IsMouseButtonReleased(0) && sq_dist > BALL_RADIUS * BALL_RADIUS)
            {
                hit_ball(&(*balls)[0], -stick.normal_x * sq_dist, -stick.normal_y * sq_dist);
                game_state = HIT;
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
            check_pockets(balls, game_state, turn);
            update_balls(balls, dt);
            // change state to NOT_HIT if no handball
            // change state to HANDBALL if handball
            // TODO: handball
            if (!is_moving(balls))
            {
                if ((first_collision != turn && turn != NONE) || first_collision == NONE)
                {
                    game_state = HANDBALL;
                    turn = turn == SOLID ? STRIPES : SOLID;
                }
                else
                {
                    game_state = NOT_HIT;
                }
            }
            break;
        case HANDBALL:
            // move ball input
            // change state when ball moved

            (*balls)[0].x = mouse.x;

            (*balls)[0].y = mouse.y;

            if (IsMouseButtonDown(0))
            {
                if (in_bounds(mouse.x, mouse.y))
                {
                    game_state = NOT_HIT;
                }
            }
            break;
        case WIN_SOLID:
            // restart
            // display result, restart btn, stats, etc
            break;
        case WIN_STRIPE:
            // restart
            // display result, restart btn, stats, etc
            break;
        default:
            break;
        }

        BeginDrawing();
        // draw board
        // draw balls
        draw_table(texture);
        draw_balls(balls);
        draw_stick(&stick);

        DrawText(TextFormat("Current Game State: %s", gameStateNames[game_state]), 125, 50, 20, DARKGRAY);
        DrawText(TextFormat("Current turn is: %s", typeNames[turn]), 125, 100, 20, DARKGRAY);

        DrawText(TextFormat("Current mouse position is: (%d, %d)", (int)mouse.x, (int)mouse.y), 125, 150, 20, DARKGRAY);

        // draw stick
        ClearBackground(BLACK);
        EndDrawing();

        dt = GetFrameTime();
    }

    free(balls);
    UnloadTexture(texture);
    CloseWindow();

    return 0;
}
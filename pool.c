#include "pool.h"
#include "include/raylib.h"
#include <math.h>

Color get_ball_color(int num)
{
    switch (num)
    {
    case 0:
        return RAYWHITE; // cue ball
    case 1:
        return YELLOW;
    case 2:
        return BLUE;
    case 3:
        return RED;
    case 4:
        return PURPLE;
    case 5:
        return ORANGE;
    case 6:
        return GREEN;
    case 7:
        return MAROON;
    case 8:
        return BLACK;

    case 9:
        return YELLOW;
    case 10:
        return BLUE;
    case 11:
        return RED;
    case 12:
        return PURPLE;
    case 13:
        return ORANGE;
    case 14:
        return GREEN;
    case 15:
        return MAROON;

    default:
        return GRAY;
    }
}

Ball init_ball(int num, enum Type type, float x, float y)
{
    Ball b = {0};

    b.num = num;
    b.type = type;
    b.pos.x = x;
    b.pos.y = y;

    b.vx = 0.0f;
    b.vy = 0.0f;

    b.ax = 0.0f;
    b.ay = 0.0f;

    b.rx = 0.0f;
    b.ry = 0.0f;

    b.color = get_ball_color(num);

    b.pocketed = false;

    return b;
}

void init_balls(Ball (*balls)[BALL_COUNT])
{
    float x = RACK_START_X;
    float y = RACK_START_Y;

    // cue ball
    (*balls)[0] = init_ball(0, TYPE_CUE, CUE_START_X, CUE_START_Y);

    // rack:
    //
    //          1
    //        9   2
    //      10  8   3
    //     4  11  12  5
    //   13  6   14  15  7

    (*balls)[1] = init_ball(1, TYPE_SOLID, x, y);

    (*balls)[9] = init_ball(9, TYPE_STRIPES, x + RACK_STEP_X, y - RACK_STEP_Y);
    (*balls)[2] = init_ball(2, TYPE_SOLID, x + RACK_STEP_X, y + RACK_STEP_Y);

    (*balls)[10] = init_ball(10, TYPE_STRIPES, x + RACK_STEP_X * 2.0f, y - RACK_STEP_Y * 2.0f);
    (*balls)[8] = init_ball(8, TYPE_EIGHT, x + RACK_STEP_X * 2.0f, y);
    (*balls)[3] = init_ball(3, TYPE_SOLID, x + RACK_STEP_X * 2.0f, y + RACK_STEP_Y * 2.0f);

    (*balls)[4] = init_ball(4, TYPE_SOLID, x + RACK_STEP_X * 3.0f, y - RACK_STEP_Y * 3.0f);
    (*balls)[11] = init_ball(11, TYPE_STRIPES, x + RACK_STEP_X * 3.0f, y - RACK_STEP_Y);
    (*balls)[12] = init_ball(12, TYPE_STRIPES, x + RACK_STEP_X * 3.0f, y + RACK_STEP_Y);
    (*balls)[5] = init_ball(5, TYPE_SOLID, x + RACK_STEP_X * 3.0f, y + RACK_STEP_Y * 3.0f);

    (*balls)[13] = init_ball(13, TYPE_STRIPES, x + RACK_STEP_X * 4.0f, y - RACK_STEP_Y * 4.0f);
    (*balls)[6] = init_ball(6, TYPE_SOLID, x + RACK_STEP_X * 4.0f, y - RACK_STEP_Y * 2.0f);
    (*balls)[14] = init_ball(14, TYPE_STRIPES, x + RACK_STEP_X * 4.0f, y);
    (*balls)[15] = init_ball(15, TYPE_STRIPES, x + RACK_STEP_X * 4.0f, y + RACK_STEP_Y * 2.0f);
    (*balls)[7] = init_ball(7, TYPE_SOLID, x + RACK_STEP_X * 4.0f, y + RACK_STEP_Y * 4.0f);
}

void update_balls(Ball (*balls)[BALL_COUNT], float dt)
{
    for (int i = 0; i < BALL_COUNT; i++)
    {
        Ball *ball = &(*balls)[i];

        if (ball->pocketed)
            continue;

        ball->pos.x += ball->vx * dt;
        ball->pos.y += ball->vy * dt;

        ball->vx += ball->ax * dt;
        ball->vy += ball->ay * dt;

        ball->rx += (ball->vx / BALL_RADIUS) * dt;
        ball->ry += (ball->vy / BALL_RADIUS) * dt;

        ball->ax = -ball->vx * FRICTION_PER_FRAME;
        ball->ay = -ball->vy * FRICTION_PER_FRAME;

        if (fabsf(ball->vx) <= STOP_SPEED && fabsf(ball->ax) <= STOP_SPEED)
        {
            ball->vx = 0.0f;
        }

        if (fabsf(ball->vy) <= STOP_SPEED && fabsf(ball->ay) <= STOP_SPEED)
        {
            ball->vy = 0.0f;
        }
    }
}

float sq_distance(float x1, float y1, float x2, float y2)
{
    return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}

float distance(float x1, float y1, float x2, float y2)
{
    return (float)sqrt(sq_distance(x1, y1, x2, y2));
}

bool in_bounds(float x, float y)
{
    return x >= TABLE_LEFT &&
           x <= TABLE_RIGHT &&
           y >= TABLE_TOP &&
           y <= TABLE_BOTTOM;
}

void handle_ball_collision(Ball *ball1, Ball *ball2)
{

    float normal_x;
    float normal_y;

    float sq_dist = sq_distance(
        ball1->pos.x, ball1->pos.y,
        ball2->pos.x, ball2->pos.y);

    float min_dist = BALL_DIAMETER;

    if (sq_dist <= 0.0001f)
    {
        ball1->pos.x -= BALL_RADIUS * 0.5f;
        ball2->pos.x += BALL_RADIUS * 0.5f;
        return;
    }

    float dist = sqrtf(sq_dist);

    if (dist < min_dist)
    {
        float diff_x = ball2->pos.x - ball1->pos.x;
        float diff_y = ball2->pos.y - ball1->pos.y;

        normal_x = diff_x / dist;
        normal_y = diff_y / dist;

        float overlap = min_dist - dist;
        float correction = overlap * 0.5f;

        ball1->pos.x -= normal_x * correction;
        ball1->pos.y -= normal_y * correction;

        ball2->pos.x += normal_x * correction;
        ball2->pos.y += normal_y * correction;
    }

    // elastic response
    float p = (ball1->vx * normal_x + ball1->vy * normal_y) - (ball2->vx * normal_x + ball2->vy * normal_y);

    p *= BALL_RESTITUTION;

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
        Ball *ball = &(*balls)[i];

        if (ball->pocketed)
            continue;

        // left wall
        if (ball->pos.x - BALL_RADIUS <= walls[2].start.x && ball->vx < 0.0f)
        {
            ball->pos.x = walls[2].start.x + BALL_RADIUS;
            ball->vx = -ball->vx * WALL_RESTITUTION;
        }

        // right wall
        if (ball->pos.x + BALL_RADIUS >= walls[3].start.x && ball->vx > 0.0f)
        {
            ball->pos.x = walls[3].start.x - BALL_RADIUS;
            ball->vx = -ball->vx * WALL_RESTITUTION;
        }

        // top wall
        if (ball->pos.y - BALL_RADIUS <= walls[0].start.y && ball->vy < 0.0f)
        {
            ball->pos.y = walls[0].start.y + BALL_RADIUS;
            ball->vy = -ball->vy * WALL_RESTITUTION;
        }

        // bottom wall
        if (ball->pos.y + BALL_RADIUS >= walls[1].start.y && ball->vy > 0.0f)
        {
            ball->pos.y = walls[1].start.y - BALL_RADIUS;
            ball->vy = -ball->vy * WALL_RESTITUTION;
        }

        // ball collisions
        for (int j = i + 1; j < BALL_COUNT; j++)
        {
            float x1, x2, y1, y2;
            x1 = (*balls)[i].pos.x;
            x2 = (*balls)[j].pos.x;
            y1 = (*balls)[i].pos.y;
            y2 = (*balls)[j].pos.y;
            if (sq_distance(x1, y1, x2, y2) <= (BALL_DIAMETER * BALL_DIAMETER))
            {
                if ((*balls)[i].num == 0 && *fc == TYPE_NONE)
                {
                    *fc = (*balls)[j].type;
                }
                handle_ball_collision(&(*balls)[i], &(*balls)[j]);
            }
        }
    }
    return;
}

void check_pockets(Ball (*balls)[BALL_COUNT], enum GameState *new_state, enum Type *turn, bool *has_pocketed)
{
    for (int i = 0; i < BALL_COUNT; ++i)
    {
        Ball *ball = &(*balls)[i];

        if (ball->pocketed)
            continue;

        for (int j = 0; j < POCKET_COUNT; ++j)
        {
            const Pocket *pocket = &pockets[j];

            float dx = ball->pos.x - pocket->pos.x;
            float dy = ball->pos.y - pocket->pos.y;
            float dist_squared = dx * dx + dy * dy;

            float catch_radius = pocket->radius + BALL_RADIUS * 0.35f;

            if (dist_squared <= catch_radius * catch_radius)
            {
                handle_pocket(turn, ball, new_state, balls, has_pocketed);
                break;
            }
        }
    }
}

void declare_win(enum Type *turn, enum GameState *gs)
{
    if (*turn == TYPE_SOLID)
    {
        *gs = WIN_SOLID;
    }
    else
    {
        *gs = WIN_STRIPE;
    }
}

void handle_pocket(enum Type *turn, Ball *pocketed, enum GameState *new_state, Ball (*balls)[BALL_COUNT], bool *has_pocketed)
{
    if (pocketed->type == TYPE_CUE)
    {
        pocketed->vx = 0.0f;
        pocketed->vy = 0.0f;
        pocketed->ax = 0.0f;
        pocketed->ay = 0.0f;

        pocketed->pocketed = false;
        *new_state = HANDBALL;
        return;
    }

    pocketed->pocketed = true;
    pocketed->vx = 0.0f;
    pocketed->vy = 0.0f;
    pocketed->ax = 0.0f;
    pocketed->ay = 0.0f;

    if (pocketed->type == TYPE_EIGHT)
    {
        declare_win(turn, new_state);
        return;
    }

    if (*turn == TYPE_NONE)
    {
        *turn = pocketed->type;
        *has_pocketed = true;
        *new_state = NOT_HIT;
        return;
    }

    if (*turn != pocketed->type)
    {
        *new_state = HANDBALL;
        return;
    }

    *has_pocketed = true;
    *new_state = NOT_HIT;
}

// void draw_pockets()
// {
//     for (int i = 0; i < POCKET_COUNT; i++)
//     {
//         DrawCircleV(
//             (Vector2){pockets[i].pos.x, pockets[i].pos.y},
//             pockets[i].radius,
//             RAYWHITE);
//     }
// }

RenderTexture2D create_ball_surface(Color color, int number, bool striped)
{
    RenderTexture2D surface = LoadRenderTexture(
        BALL_SURFACE_WIDTH,
        BALL_SURFACE_HEIGHT);

    int w = BALL_SURFACE_WIDTH;
    int h = BALL_SURFACE_HEIGHT;

    BeginTextureMode(surface);

    if (striped)
    {
        ClearBackground(RAYWHITE);

        DrawRectangle(
            0,
            (int)(h * 0.35f),
            w,
            (int)(h * 0.30f),
            color);
    }
    else
    {
        ClearBackground(color);
    }

    if (number > 0)
    {
        float badgeRadius = h * 0.16f;
        int fontSize = (int)(h * 0.18f);

        char text[4];
        snprintf(text, sizeof(text), "%d", number);

        int textWidth = MeasureText(text, fontSize);

        DrawCircle(
            w / 2,
            h / 2,
            badgeRadius,
            RAYWHITE);

        DrawText(
            text,
            w / 2 - textWidth / 2,
            h / 2 - fontSize / 2,
            fontSize,
            BLACK);
    }

    EndTextureMode();

    return surface;
}

void draw_ball_number(Ball *ball)
{
    if (ball->num == 0)
        return;

    char text[4];

    snprintf(text, sizeof(text), "%d", ball->num);

    int fontSize = 9;
    int textWidth = MeasureText(text, fontSize);

    float speed = sqrtf(ball->vx * ball->vx + ball->vy * ball->vy);

    float markerX = ball->pos.x + sinf(ball->rx) * BALL_RADIUS * 0.45f;
    float markerY = ball->pos.y + sinf(ball->ry) * BALL_RADIUS * 0.45f;

    float dx = markerX - ball->pos.x;
    float dy = markerY - ball->pos.y;

    if (dx * dx + dy * dy < BALL_RADIUS * BALL_RADIUS * 0.25f)
    {
        // draw number / marker
        DrawCircleV(
            (Vector2){markerX, markerY},
            BALL_RADIUS * 0.45f,
            RAYWHITE);

        DrawText(
            text,
            (int)(markerX - textWidth / 2),
            (int)(markerY - fontSize / 2),
            fontSize,
            BLACK);
    }
}

void draw_balls(Ball (*balls)[BALL_COUNT])
{
    for (int i = 0; i < BALL_COUNT; i++)
    {
        Ball *ball = &(*balls)[i];

        if (ball->pocketed)
            continue;

        if (ball->type == TYPE_STRIPES)
        {
            DrawCircleV(
                (Vector2){ball->pos.x, ball->pos.y},
                BALL_RADIUS,
                WHITE);

            struct Vector2 start;
            struct Vector2 end;

            start.x = (int)(ball->pos.x - BALL_RADIUS);
            start.y = (int)(ball->pos.y);

            end.x = (int)(ball->pos.x + BALL_RADIUS);
            end.y = (int)(ball->pos.y);

            DrawLineEx(start, end, BALL_RADIUS, ball->color);
        }
        else
        {
            DrawCircleV(
                (Vector2){ball->pos.x, ball->pos.y},
                BALL_RADIUS,
                ball->color);
        }

        draw_ball_number(ball);
    }
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

void switch_turn(enum Type *turn)
{
    *turn = *turn == TYPE_SOLID ? TYPE_STRIPES : TYPE_SOLID;
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

    struct Vector2 point_start;
    struct Vector2 point_end;

    start.x = stick->start_pos.x;
    start.y = stick->start_pos.y;
    end.x = stick->end_pos.x;
    end.y = stick->end_pos.y;

    point_start.x = start.x + stick->normal_x * (STICK_LENGTH * 0.01);
    point_start.y = start.y + stick->normal_y * (STICK_LENGTH * 0.01);

    DrawLineEx(start, end, 5, WHITE);

    DrawLineEx(end, point_start, 5, DARKGRAY);

    return;
};

void set_stick(Stick *stick, float mouse_x, float mouse_y, float wb_x, float wb_y)
{

    float dist = distance(mouse_x, mouse_y, wb_x, wb_y);

    float diff_x = mouse_x - wb_x;
    float diff_y = mouse_y - wb_y;

    if (dist <= BALL_RADIUS * 1.2)
    {
        // set angle of the shot
        stick->normal_x = diff_x / dist;
        stick->normal_y = diff_y / dist;
        stick->start_pos.x = wb_x + stick->normal_x * BALL_RADIUS;
        stick->start_pos.y = wb_y + stick->normal_y * BALL_RADIUS;
        stick->end_pos.x = wb_x - stick->normal_x * -STICK_LENGTH;
        stick->end_pos.y = wb_y - stick->normal_y * -STICK_LENGTH;
    }
    else
    {
        // set power of the shot
        stick->start_pos.x = wb_x + stick->normal_x * BALL_RADIUS + stick->normal_x * dist;
        stick->start_pos.y = wb_y + stick->normal_y * BALL_RADIUS + stick->normal_y * dist;
        stick->end_pos.x = wb_x - stick->normal_x * -STICK_LENGTH + stick->normal_x * dist;
        stick->end_pos.y = wb_y - stick->normal_y * -STICK_LENGTH + stick->normal_y * dist;
    }

    return;
}

void draw_modal()
{

    float width = GetScreenWidth() * 0.92, height = GetScreenHeight() * 0.83;

    DrawRectangle(GetScreenWidth() / 2.0f - width / 2, GetScreenHeight() / 2.0f - height / 2, width, height, LIGHTGRAY);
}

int main(void)
{

    enum GameState game_state = NOT_HIT;
    enum GameState new_state = NOT_HIT;
    enum Type first_collision = TYPE_NONE;
    enum Type turn = TYPE_NONE;
    bool has_pocketed = false;

    bool show_modal = false;
    bool show_win = false;
    bool show_details = true;

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

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        mouse = GetMousePosition();

        if (IsKeyPressed(KEY_I))
        {
            show_modal = !show_modal;
        }

        switch (game_state)
        {

        case NOT_HIT:
            // stick input
            stick.hide = true;

            if (IsMouseButtonDown(0))
            {
                stick.hide = false;

                set_stick(&stick, mouse.x, mouse.y, (*balls)[0].pos.x, (*balls)[0].pos.y);
            }

            float sq_dist = sq_distance(mouse.x, mouse.y, (*balls)[0].pos.x, (*balls)[0].pos.y);
            if (IsMouseButtonReleased(0) && sq_dist > BALL_RADIUS * BALL_RADIUS)
            {
                hit_ball(&(*balls)[0], -stick.normal_x * sq_dist, -stick.normal_y * sq_dist);
                game_state = HIT;
            }

            break;
        case HIT:

            update_balls(balls, dt);
            check_pockets(balls, &new_state, &turn, &has_pocketed);
            check_collisions(balls, &first_collision);

            if (!is_moving(balls))
            {

                if ((first_collision != turn && turn != TYPE_NONE) || first_collision == TYPE_NONE)
                {
                    game_state = HANDBALL;
                }
                else
                {
                    game_state = new_state;
                }

                first_collision = TYPE_NONE;

                if (!has_pocketed)
                    switch_turn(&turn);

                has_pocketed = false;
            }
            break;
        case HANDBALL:
            // move ball input
            // change state when ball moved

            (*balls)[0].pos.x = mouse.x;

            (*balls)[0].pos.y = mouse.y;

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
        // draw_pockets();
        draw_balls(balls);
        draw_stick(&stick);

        if (show_modal)
        {
            draw_modal();

            if (show_details)
            {
                DrawText(TextFormat("Current Game State: %s", gameStateNames[game_state]), 125, 50, 20, DARKGRAY);
                DrawText(TextFormat("Current turn is: %s", typeNames[turn]), 125, 100, 20, DARKGRAY);

                DrawText(TextFormat("Current mouse position is: (%d, %d)", (int)mouse.x, (int)mouse.y), 125, 150, 20, DARKGRAY);
            }
        }

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

#ifndef BILLIARDS_H
#define BILLIARDS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "include/raylib.h"

#define SCREEN_WIDTH 750
#define SCREEN_HEIGHT 400

#define BALL_COUNT 16
#define BALL_DIAMETER 25.0f
#define BALL_RADIUS (BALL_DIAMETER / 2.0f)

#define WALL_COUNT 4
#define POCKET_COUNT 6

#define FPS 1000.0f
#define DELTA 0.5f

#define FRICTION_PER_FRAME 0.994f
#define WALL_RESTITUTION 0.9f
#define BALL_RESTITUTION 0.95f
#define STOP_SPEED 0.2f

#define STICK_LENGTH 400.0f

// Table collision bounds
#define TABLE_LEFT 31.0f
#define TABLE_RIGHT 721.0f
#define TABLE_TOP 24.0f
#define TABLE_BOTTOM 370.0f

#define TABLE_CENTER_X ((TABLE_LEFT + TABLE_RIGHT) / 2.0f)
#define TABLE_CENTER_Y ((TABLE_TOP + TABLE_BOTTOM) / 2.0f)

// Starting positions
#define CUE_START_X (TABLE_LEFT + ((TABLE_RIGHT - TABLE_LEFT) / 4.0f))
#define CUE_START_Y TABLE_CENTER_Y

#define RACK_START_X (TABLE_LEFT + (((TABLE_RIGHT - TABLE_LEFT) * 3.0f) / 4.0f))
#define RACK_START_Y TABLE_CENTER_Y

#define RACK_STEP_X BALL_RADIUS * 1.8f
#define RACK_STEP_Y BALL_RADIUS * 1.1f

enum Type
{
    TYPE_SOLID,
    TYPE_STRIPES,
    TYPE_CUE,
    TYPE_EIGHT,
    TYPE_NONE,
    TYPE_COUNT,
};

static const char *const typeNames[TYPE_COUNT] = {
    "SOLID",
    "STRIPES",
    "CUE",
    "EIGHT",
    "NONE",
};

enum GameState
{
    NOT_HIT,
    HIT,
    HANDBALL,
    WIN_SOLID,
    WIN_STRIPE,
    GAME_STATE_COUNT,
};

static const char *const gameStateNames[GAME_STATE_COUNT] = {
    "NOT_HIT",
    "HIT",
    "HANDBALL",
    "WIN_SOLID",
    "WIN_STRIPE",
};

typedef struct Vec
{
    float x;
    float y;
} Vec;

typedef struct Ball
{
    int num;
    enum Type type;
    Color color;
    bool pocketed;

    Vec pos;

    // velocity
    float vx;
    float vy;

    // acceleration
    float ax;
    float ay;

    // rotation
    float rx;
    float ry;

} Ball;

typedef struct Stick
{
    Vec start_pos;
    Vec end_pos;

    float normal_x;
    float normal_y;

    bool hide;
} Stick;

typedef struct Pocket
{
    Vec pos;
    float radius;
} Pocket;

typedef struct Wall
{
    Vec start;
    Vec end;
    Vec normal;
} Wall;

static const Wall walls[WALL_COUNT] = {
    {{TABLE_LEFT, TABLE_TOP}, {TABLE_RIGHT, TABLE_TOP}, {0.0f, 1.0f}},        // top
    {{TABLE_LEFT, TABLE_BOTTOM}, {TABLE_RIGHT, TABLE_BOTTOM}, {0.0f, -1.0f}}, // bottom
    {{TABLE_LEFT, TABLE_BOTTOM}, {TABLE_LEFT, TABLE_TOP}, {1.0f, 0.0f}},      // left
    {{TABLE_RIGHT, TABLE_BOTTOM}, {TABLE_RIGHT, TABLE_TOP}, {-1.0f, 0.0f}},   // right
};

static const Pocket pockets[POCKET_COUNT] = {
    {{34.0f, 29.0f}, BALL_RADIUS * 1.8f},  // top-left
    {{375.0f, 20.0f}, BALL_RADIUS * 1.8f}, // top-middle
    {{722.0f, 26.0f}, BALL_RADIUS * 1.8f}, // top-right

    {{32.0f, 371.0f}, BALL_RADIUS * 1.8f},  // bottom-left
    {{376.0f, 369.0f}, BALL_RADIUS * 1.8f}, // bottom-middle
    {{720.0f, 369.0f}, BALL_RADIUS * 1.8f}, // bottom-right
};

Color get_ball_color(int num);

Ball init_ball(int num, enum Type type, float x, float y);
void init_balls(Ball (*balls)[BALL_COUNT]);

void update_balls(Ball (*balls)[BALL_COUNT], float dt);

float sq_distance(float x1, float y1, float x2, float y2);
float distance(float x1, float y1, float x2, float y2);

bool in_bounds(float x, float y);
bool is_moving(Ball (*balls)[BALL_COUNT]);

void handle_ball_collision(Ball *ball1, Ball *ball2);
void check_collisions(Ball (*balls)[BALL_COUNT], enum Type *first_collision);

void check_pockets(
    Ball (*balls)[BALL_COUNT],
    enum GameState *new_state,
    enum Type *turn,
    bool *has_pocketed);

void handle_pocket(
    enum Type *turn,
    Ball *pocketed,
    enum GameState *new_state,
    Ball (*balls)[BALL_COUNT],
    bool *has_pockted);

void declare_win(enum Type *turn, enum GameState *game_state);
void switch_turn(enum Type *turn);

void draw_pockets(void);
void draw_ball_number(Ball *ball);
void draw_balls(Ball (*balls)[BALL_COUNT]);
void draw_table(Texture2D table_texture);
void draw_stick(Stick *stick);

void hit_ball(Ball *white_ball, float force_x, float force_y);

void set_stick(
    Stick *stick,
    float mouse_x,
    float mouse_y,
    float wb_x,
    float wb_y);
#endif
#ifndef BILLIARDS_H
#define BILLIARDS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SCREEN_WIDTH 750
#define SCREEN_HEIGHT 400

#define BALL_COUNT 16
#define BALL_DIAMETER 20.0f
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

#define RACK_STEP_X 18.0f
#define RACK_STEP_Y 11.0f

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

    Vec pos;

    float vx;
    float vy;

    float av;
    float ax;
    float ay;

    bool pocketed;
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
    {{TABLE_LEFT, TABLE_TOP}, {TABLE_RIGHT, TABLE_TOP}, {0.0f, 1.0f}},       // top
    {{TABLE_LEFT, TABLE_BOTTOM}, {TABLE_RIGHT, TABLE_BOTTOM}, {0.0f, -1.0f}}, // bottom
    {{TABLE_LEFT, TABLE_BOTTOM}, {TABLE_LEFT, TABLE_TOP}, {1.0f, 0.0f}},      // left
    {{TABLE_RIGHT, TABLE_BOTTOM}, {TABLE_RIGHT, TABLE_TOP}, {-1.0f, 0.0f}},   // right
};

static const Pocket pockets[POCKET_COUNT] = {
    {{34.0f, 29.0f}, 15.0f},    // top-left
    {{375.0f, 16.0f}, 15.0f},   // top-middle
    {{722.0f, 26.0f}, 15.0f},   // top-right

    {{32.0f, 367.0f}, 15.0f},   // bottom-left
    {{376.0f, 379.0f}, 15.0f},  // bottom-middle
    {{720.0f, 369.0f}, 15.0f},  // bottom-right
};

Ball init_ball(int num, enum Type type, float x, float y);
void init_balls(Ball (*balls)[BALL_COUNT]);

void update_balls(Ball (*balls)[BALL_COUNT], float dt);

void handle_ball_collision(Ball *ball1, Ball *ball2);
void check_collisions(Ball (*balls)[BALL_COUNT], enum Type *fc);

void check_pockets(Ball (*balls)[BALL_COUNT], enum GameState *gs, enum Type *turn);
void handle_pocket(enum Type *turn, Ball *pocketed, enum GameState *gs);

void render_balls(Ball (*balls)[BALL_COUNT]);
void render_stick(Stick *stick);

bool in_bounds(int x, int y);

void set_stick(
    Stick *stick,
    float mouse_x,
    float mouse_y,
    float wb_x,
    float wb_y
);

#endif
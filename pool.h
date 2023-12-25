#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SCREEN_WIDTH 750
#define SCREEN_HEIGHT 400
#define BALL_COUNT 2
#define BALL_DIAMETER 20.0
#define BALL_RADIUS (BALL_DIAMETER / 2.0)
#define FPS 60
#define DT 1.0/FPS


enum Type {
    SOLID,
    STRIPES,
    WHITE,
    BLACK,
    NONE,
};

enum GameState {
    NOT_HIT,
    HIT,
    HANDBALL
};

typedef struct ball {
    int num;
    enum Type type;

    float x;
    float y;

    float vx;
    float vy;

    float av;
    float a;

    bool pocketed;
} Ball;


typedef struct stick {
    float x;
} Stick;

Ball init_ball(int num, enum Type type, float x, float y);
void init_balls(Ball (*balls)[BALL_COUNT]);
void update_ball(Ball* ball);
void handle_ball_collision(Ball* ball1, Ball* ball2);
void handle_wall_collision(Ball* ball);
void check_collisions(Ball (*balls)[BALL_COUNT]);
void check_pockets(Ball (*balls)[BALL_COUNT]);
void handle_pocket(Ball *pocketed);
void render_balls(Ball (*balls)[BALL_COUNT]);
void render_stick(Stick *stick);









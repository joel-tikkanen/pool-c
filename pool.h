#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SCREEN_WIDTH 750
#define SCREEN_HEIGHT 400
#define BALL_COUNT 2
#define BALL_DIAMETER 20.0
#define BALL_RADIUS (BALL_DIAMETER / 2.0)
#define FPS 1000.0
#define FRICTION 0.5
#define STICK_LENGTH 400.0


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
    HANDBALL,
    WIN_SOLID,
    WIN_STRIPE,
};

typedef struct ball {
    int num;
    enum Type type;

    float x;
    float y;

    float vx;
    float vy;

    float av;
    float ax;
    float ay;

    bool pocketed;
} Ball;


typedef struct stick {
    float start_x;
    float start_y;
    float end_x;
    float end_y;

    float normal_x;
    float normal_y;

    bool hide;
} Stick;

Ball init_ball(int num, enum Type type, float x, float y);
void init_balls(Ball (*balls)[BALL_COUNT]);
void handle_ball_collision(Ball* ball1, Ball* ball2);
void check_collisions(Ball (*balls)[BALL_COUNT], enum Type *fc);
void check_pockets(Ball (*balls)[BALL_COUNT]);
void handle_pocket(enum Type turn, Ball *pocketed, enum GameState *gs);
void render_balls(Ball (*balls)[BALL_COUNT]);
void render_stick(Stick *stick);
void set_stick(Stick* stick, float mouse_x, float mouse_y,float wb_x, float wb_y);

struct Vector2 GetMousePosition();








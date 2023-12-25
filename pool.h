#include <stdbool.h>

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 500
#define BALL_COUNT 16


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
}

typedef struct ball {
    float radius;

    int x;
    int y;

    float vx;
    float vy;

    float av;

    bool pocketed;
} Ball;


typedef struct stick {
    float x;
} Stick;

void init_ball(int num, enum GameState gs);
void init_balls(Ball *balls[BALL_COUNT]);
void update_ball(Ball* ball);
void handle_ball_collision(Ball* ball1, Ball* ball2);
void handle_wall_collision(Ball* ball);
void check_collisions(Ball* balls[BALL_COUNT]);
void check_pockets(Ball *balls[BALL_COUNT]);
void handle_pocket(Ball *pocketed);
void render_ball(Ball *ball);
void render_stick(Stick *stick);









#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 500


enum Type {
    SOLID,
    STRIPES,
    WHITE,
    BLACK,
};

typedef struct ball {

    float radius;

    int x;
    int y;

    float vx;
    float vy;

    float av;

} Ball;
#include "raylib.h"
#include <deque>

struct Body {
    float mass;
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;

    std::deque<Vector2> prev_positions;

    Body(float mass, Vector2 initialPosition, Vector2 initialVelocity);
};
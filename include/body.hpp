#include "raylib.h"
#include <deque>

struct Body {
    float mass;
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;

    float radius;
    std::deque<Vector2> prev_positions;
    bool is_destroyed;

    Body(float mass, Vector2 initialPosition, Vector2 initialVelocity);

    float calculate_radius();
};
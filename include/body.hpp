#include "raylib.h"

struct Body {
    float mass;
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;

    Body(float mass, Vector2 initialPosition, Vector2 initialVelocity);
};
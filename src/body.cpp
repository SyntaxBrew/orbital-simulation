#include "body.hpp"
#include "raylib.h"
#include "raymath.h"

Body::Body(float mass, Vector2 initialPosition, Vector2 initialVelocity) {
    this->mass = mass;
    this->position = initialPosition;
    this->velocity = initialVelocity;
    this->radius = powf(this->mass, 0.2) * 6;
    this->is_destroyed = false;
}
#include "body.hpp"
#include "raylib.h"

Body::Body(float mass, Vector2 initialPosition, Vector2 initialVelocity) {
    this->mass = mass;
    this->position = initialPosition;
    this->velocity = initialVelocity;
}

void Body::update() {

}

void Body::draw() {
    
}
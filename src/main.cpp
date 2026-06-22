#include "raylib.h"
#include "raymath.h"

#include <iostream>
#include <vector>

#include "body.hpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

const float GRAV_CONST = 1000;

int main() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Orbital Simulation");
    SetTargetFPS(60);

    std::vector<Body> static_bodies{};
    std::vector<Body> dynamic_bodies{};

    static_bodies.push_back(
        Body(
            10000.0f,
            {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2},
            {0, 0}
        )
    );

    dynamic_bodies.push_back(
        Body(
            100.0f,
            {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 256},
            {197.642, 0}
        )
    );

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        float deltaTime = GetFrameTime();

        for (Body& dynamic_body: dynamic_bodies) {
            for (Body& static_body: static_bodies) {
                float dx = dynamic_body.position.x - static_body.position.x;
                float dy = dynamic_body.position.y - static_body.position.y;

                std::cout << "(" << sqrtf(dx * dx + dy * dy) << ")\n";
    
                float angle = atan2f(dy, dx);
                float grav_force_magnitude = GRAV_CONST * (dynamic_body.mass * static_body.mass) / (dx * dx + dy * dy);

                Vector2 grav_force = Vector2{-grav_force_magnitude * cosf(angle), -grav_force_magnitude * sinf(angle)};
                dynamic_body.acceleration = Vector2Scale(grav_force, 1 / dynamic_body.mass);

                DrawLineEx(dynamic_body.position, static_body.position, 4.0f, BLUE);
            } 

            dynamic_body.velocity = Vector2Add(dynamic_body.velocity, Vector2Scale(dynamic_body.acceleration, deltaTime));
            dynamic_body.position = Vector2Add(dynamic_body.position, Vector2Scale(dynamic_body.velocity, deltaTime));
        }

        for (Body& body: static_bodies) {
            DrawCircle(body.position.x, body.position.y, 32, RED);
        }
        for (Body& body: dynamic_bodies) {
            DrawCircle(body.position.x, body.position.y, 16, GREEN);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
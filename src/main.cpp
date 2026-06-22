#include "raylib.h"
#include "raymath.h"

#include <iostream>
#include <vector>

#include "body.hpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

int main() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Orbital Simulation");
    SetTargetFPS(60);

    std::vector<Body> staticBodies{};
    std::vector<Body> dynamicBodies{};

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        float deltaTime = GetFrameTime();

        for (Body& body: dynamicBodies) {
            body.velocity = Vector2Add(body.velocity, Vector2Scale(body.acceleration, deltaTime));
            body.position = Vector2Add(body.position, Vector2Scale(body.velocity, deltaTime));
        }

        for (Body& body: dynamicBodies) {
            DrawCircle(body.position.x, body.position.y, 16, GREEN);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
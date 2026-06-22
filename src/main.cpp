#include "raylib.h"
#include <iostream>

#include "body.hpp"

int main() {
    InitWindow(800, 800, "Orbital Simulation");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
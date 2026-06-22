#include "raylib.h"
#include <iostream>

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
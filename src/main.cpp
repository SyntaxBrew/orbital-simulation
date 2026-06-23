#include "raylib.h"
#include "raymath.h"

#include <iostream>
#include <vector>
#include <algorithm>

#include "body.hpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

const float GRAV_CONST = 1000;

const bool ARROWS_ENABLED = false;
const float ARROW_LENGTH = 64;
const float ARROW_HEAD_LENGTH = 24;
const float ARROW_HEAD_SIZE = 6;

const int MAX_TRAIL_SIZE = 128;

float vector2_magnitudef(Vector2 vector2) {
    return sqrtf(vector2.x * vector2.x + vector2.y * vector2.y);
}

int main() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Orbital Simulation");
    SetTargetFPS(60);

    std::vector<Body> static_bodies{};
    std::vector<Body> dynamic_bodies{};

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            dynamic_bodies.push_back(
                Body(
                    100.0f,
                    GetMousePosition(),
                    {197.642, 0}
                )
            );
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            static_bodies.push_back(
                Body(
                    10000.0f,
                    GetMousePosition(),
                    {0, 0}
                )
            );
        }

        for (Body& dynamic_body: dynamic_bodies) {
            Vector2 total_acceleration{0, 0};

            for (Body& static_body: static_bodies) {
                float dx = dynamic_body.position.x - static_body.position.x;
                float dy = dynamic_body.position.y - static_body.position.y;
                float distance = std::fmax(sqrtf(dx * dx + dy * dy), 32);
    
                float angle = atan2f(dy, dx);
                float grav_force_magnitude = GRAV_CONST * (dynamic_body.mass * static_body.mass) / (distance * distance);

                Vector2 grav_force{-grav_force_magnitude * cosf(angle), -grav_force_magnitude * sinf(angle)};
                total_acceleration = Vector2Add(total_acceleration, Vector2Scale(grav_force, 1 / dynamic_body.mass));
            } 

            dynamic_body.acceleration = total_acceleration;
            dynamic_body.velocity = Vector2Add(dynamic_body.velocity, Vector2Scale(dynamic_body.acceleration, deltaTime));
            dynamic_body.position = Vector2Add(dynamic_body.position, Vector2Scale(dynamic_body.velocity, deltaTime));

            if (dynamic_body.prev_positions.size() >= MAX_TRAIL_SIZE) {
                dynamic_body.prev_positions.pop_front();
            }
            dynamic_body.prev_positions.push_back(dynamic_body.position);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        for (Body& body: static_bodies) {
            DrawCircle(body.position.x, body.position.y, 32, RED);
        }
      
        for (Body& body: dynamic_bodies) {

            if (ARROWS_ENABLED) {
                float magnitude = vector2_magnitudef(body.acceleration);
                Vector2 force_dir{body.acceleration.x / magnitude, body.acceleration.y / magnitude};
                Vector2 end_position = Vector2Add(body.position, Vector2Scale(force_dir, ARROW_LENGTH));
                DrawLineEx(body.position, end_position, 4.0f, BLUE);

                Vector2 perp_dir{-force_dir.y, force_dir.x};
                Vector2 head_position{Vector2Add(end_position, Vector2Scale(force_dir, ARROW_HEAD_LENGTH))};
            
                DrawTriangle(
                    head_position,
                    Vector2{ end_position.x - perp_dir.x * ARROW_HEAD_SIZE, end_position.y - perp_dir.y * ARROW_HEAD_SIZE },
                    Vector2{ end_position.x + perp_dir.x * ARROW_HEAD_SIZE, end_position.y + perp_dir.y * ARROW_HEAD_SIZE },
                    BLUE
                );
            }
            

            for (int i = 1; i < body.prev_positions.size(); i++) {
                Vector2 start_pos = body.prev_positions.at(i-1);
                Vector2 end_pos = body.prev_positions.at(i);

                float alpha = (float) i / body.prev_positions.size();
                DrawLineEx(start_pos, end_pos, 2.0f, Fade(BLUE, alpha));
            }
            
            DrawCircle(body.position.x, body.position.y, 16, GREEN);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
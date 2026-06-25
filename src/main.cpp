#include "raylib.h"
#include "raymath.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#include "body.hpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

const float GRAV_CONST = 768;

const bool ARROWS_ENABLED = true;
const float ARROW_LENGTH = 64;
const float ARROW_HEAD_LENGTH = 24;
const float ARROW_HEAD_SIZE = 6;

const int MAX_TRAIL_SIZE = 128;

const Vector2 SCREEN_CENTER{WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f};

float vector2_magnitudef(Vector2 vector2) {
    return sqrtf(vector2.x * vector2.x + vector2.y * vector2.y);
}

Vector2 calculate_grav_acceleration(Body& body_a, Body& body_b) {
    float dx = body_b.position.x - body_a.position.x;
    float dy = body_b.position.y - body_a.position.y;
    float distance = std::fmax(sqrtf(dx * dx + dy * dy), 32);

    float angle = atan2f(dy, dx);
    float grav_force_magnitude = GRAV_CONST * (body_b.mass * body_a.mass) / (distance * distance);

    Vector2 grav_force{-grav_force_magnitude * cosf(angle), -grav_force_magnitude * sinf(angle)};
    return Vector2Scale(grav_force, 1.0f / body_b.mass);
}

Vector2 to_screen_vector(Vector2 world_vector, Vector2 camera_pos, float camera_scale) {
    return Vector2Add(Vector2Scale(Vector2Subtract(world_vector, camera_pos), camera_scale), SCREEN_CENTER);
}

Vector2 to_world_vector(Vector2 screen_vector, Vector2 camera_pos, float camera_scale) {
    return Vector2Add(Vector2Scale(Vector2Subtract(screen_vector, SCREEN_CENTER), 1.0f / camera_scale), camera_pos);
}

int main() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Orbital Simulation");
    SetTargetFPS(60);

    std::vector<Body> static_bodies{};
    std::vector<Body> dynamic_bodies{};

    float current_velocity{0};
    float current_mass{100};
    
    Vector2 camera_pos{0, 0};
    float camera_scale{1};

    int default_text_size = GuiGetStyle(DEFAULT, TEXT_SIZE);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        if (IsKeyPressed(KEY_ONE)) {
            dynamic_bodies.push_back(
                Body(
                    std::fmax(1, current_mass),
                    to_world_vector(GetMousePosition(), camera_pos, camera_scale),
                    {current_velocity, 0}
                )
            );
        }

        camera_scale = std::fmax(camera_scale + 0.05 * GetMouseWheelMove(), 0.05);
        std::cout << camera_scale << "\n";

        if (IsKeyDown(KEY_W)) {
            camera_pos.y -= 4.0f / camera_scale;
        }
        if (IsKeyDown(KEY_S)) {
            camera_pos.y += 4.0f / camera_scale;
        }
        if (IsKeyDown(KEY_A)) {
            camera_pos.x -= 4.0f / camera_scale;
        }
        if (IsKeyDown(KEY_D)) {
            camera_pos.x += 4.0f / camera_scale;
        }

        std::string velocityText{"Velocity"};
        GuiSliderBar(
            {8 + (float) MeasureText(velocityText.c_str(), default_text_size), WINDOW_HEIGHT - 25-4, 100, 25},
            velocityText.c_str(),
            std::to_string(current_velocity).c_str(),
            &current_velocity,
            0,
            1000
        );

        std::string massText{"Mass    "};
        GuiSliderBar(
            {8 + (float) MeasureText(massText.c_str(), default_text_size), WINDOW_HEIGHT - 50-8, 100, 25},
            massText.c_str(),
            std::to_string(current_mass).c_str(),
            &current_mass,
            0,
            1000000
        );

        for (Body& dynamic_body: dynamic_bodies) {
            Vector2 total_acceleration{0, 0};

            for (Body& other_dynamic_body: dynamic_bodies) {
                if (&dynamic_body == &other_dynamic_body) continue;
                if (other_dynamic_body.is_destroyed) continue;
            
                if (CheckCollisionCircles(dynamic_body.position, dynamic_body.radius, other_dynamic_body.position, other_dynamic_body.radius)) {
                    // TO DO: Implement 2D momentum mechanics
                    if (dynamic_body.mass <= other_dynamic_body.mass) {
                        other_dynamic_body.mass += dynamic_body.mass;
                        other_dynamic_body.radius = other_dynamic_body.calculate_radius();
                        dynamic_body.is_destroyed = true;
                        break;
                    }
                }

                total_acceleration = Vector2Add(total_acceleration, calculate_grav_acceleration(other_dynamic_body, dynamic_body));
            }

            if (dynamic_body.is_destroyed) {
                continue;
            }

            dynamic_body.acceleration = total_acceleration;
            dynamic_body.velocity = Vector2Add(dynamic_body.velocity, Vector2Scale(dynamic_body.acceleration, deltaTime));
            dynamic_body.position = Vector2Add(dynamic_body.position, Vector2Scale(dynamic_body.velocity, deltaTime));

            if (dynamic_body.prev_positions.size() >= MAX_TRAIL_SIZE) {
                dynamic_body.prev_positions.pop_front();
            }
            dynamic_body.prev_positions.push_back(dynamic_body.position);
        }

        for (int i = dynamic_bodies.size()-1; i >= 0; i--) {
            Body body = dynamic_bodies.at(i);
            if (body.is_destroyed) {
                dynamic_bodies.erase(dynamic_bodies.begin() + i);
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);
      
        for (Body& body: dynamic_bodies) {
            if (ARROWS_ENABLED) {
                float magnitude = vector2_magnitudef(body.acceleration);
                Vector2 force_dir{body.acceleration.x / magnitude, body.acceleration.y / magnitude};
                Vector2 end_position = Vector2Add(body.position, Vector2Scale(force_dir, ARROW_LENGTH));
                DrawLineEx(
                    to_screen_vector(body.position, camera_pos, camera_scale),
                    to_screen_vector(end_position, camera_pos, camera_scale),
                    4.0f * camera_scale,
                    BLUE
                );

                Vector2 perp_dir{-force_dir.y, force_dir.x};
                Vector2 head_position{Vector2Add(end_position, Vector2Scale(force_dir, ARROW_HEAD_LENGTH))};
            
                DrawTriangle(
                    to_screen_vector(head_position, camera_pos, camera_scale),
                    to_screen_vector(Vector2{ end_position.x - perp_dir.x * ARROW_HEAD_SIZE, end_position.y - perp_dir.y * ARROW_HEAD_SIZE }, camera_pos, camera_scale),
                    to_screen_vector(Vector2{ end_position.x + perp_dir.x * ARROW_HEAD_SIZE, end_position.y + perp_dir.y * ARROW_HEAD_SIZE }, camera_pos, camera_scale),
                    BLUE
                );
            }
            
            for (int i = 1; i < body.prev_positions.size(); i++) {
                Vector2 start_pos = body.prev_positions.at(i-1);
                Vector2 end_pos = body.prev_positions.at(i);

                float alpha = (float) i / body.prev_positions.size();
                DrawLineEx(
                    to_screen_vector(start_pos, camera_pos, camera_scale),
                    to_screen_vector(end_pos, camera_pos, camera_scale),
                    2.0f * camera_scale,
                    Fade(BLUE, alpha)
                );
            }
            
            DrawCircleV(
                to_screen_vector(body.position, camera_pos, camera_scale),
                body.radius * camera_scale,
                GREEN
            );
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
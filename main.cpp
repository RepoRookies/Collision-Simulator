#include <raylib.h>

#include "src/Circle/Circle.h"
#include <vector>

int main() {
    // Window Dimensions
    const int window_width = 800;
    const int window_height = 600;

    const int nof_circles = 1;
    const double restitution = 0.9;

    const int radius = 50;
    const int position_offset = 10;

    std::vector <Vec2D> centers = {
        Vec2D(window_width / 2, window_height / 2),
        Vec2D(window_width / 2 + position_offset, window_height / 2 - position_offset)
    };

    std::vector <double> gravity = {
        500 * GetFrameTime(),
        500 * GetFrameTime()
    };

    std::vector <Vec2D> init_velocity = {
        Vec2D(0, 100),
        Vec2D(-5, 10)
    };

    std::vector <Circle> circles;
    for (int i = 0; i < nof_circles; i++) {
        circles.push_back(Circle(centers[i], radius, init_velocity[i]));
    }

    // Initialize the Window
    InitWindow(window_width, window_height, "DEEZ NUTS");

    // Set the Target FPS
    SetTargetFPS(60);

    while (!WindowShouldClose()) { 
        for (auto& circle : circles) {
            circle.handleGravity(window_height);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw Circles
        for (auto& circle : circles) {
            DrawCircle(circle.center.x_comp, circle.center.y_comp, circle.radius, circle.color);
        }

        // Overall Collision Handler
        if (circles.size() > 1) {
            Circle::handleCollision(circles[1], circles[0], window_width, window_height, restitution);
        }

        // Movement and other Important Updates
        for (auto& circle : circles) {
            circle.moveCircle();
            circle.handleWindowBounds(window_width, window_height, restitution);
        }

        // Draw other shapes or text here if needed
        DrawText("Press ESC to close", 10, 10, 20, DARKGRAY);

        EndDrawing(); // End drawing
    }

    // Close the window and OpenGL context
    CloseWindow();

    return 0;
}

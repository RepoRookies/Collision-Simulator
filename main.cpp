#include <raylib.h>

#include "src/Circle/Circle.h"
#include "src/CollissionEngine/CollissionEngine.h"
#include <vector>

int main() {
    // Window Dimensions
    const int window_width = 800;
    const int window_height = 600;

    const int nof_circles = 5;
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

    //std::vector <Vec2D> init_velocity = {
    //    Vec2D(0, 100),
    //    Vec2D(-5, 10)
    //};

    CollissionEngine::init();


    // Initialize the Window
    InitWindow(window_width, window_height, "DEEZ NUTS");

    // Set the Target FPS
    SetTargetFPS(60);

    while (!WindowShouldClose()) { 

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			vec2 mouse(GetMousePosition());
			CollissionEngine::addCircle(Circle(Vec2D(mouse.x,mouse.y), radius, Vec2D(1, 0)));
        }
        // Draw Circles
        for (auto& circle : CollissionEngine::getCircles()) {
            circle.drawGfx();
        }

        //Overall Collision Handler
        if (CollissionEngine::getCircles().size() > 1) {
            Circle::handleCollision(CollissionEngine::getCircles()[1], CollissionEngine::getCircles()[0], window_width, window_height, restitution);
        }
		//CollissionEngine::Simulate(Core::SimType::NO_HASH);

             // Draw other shapes or text here if needed
        DrawText("Press ESC to close", 10, 10, 20, DARKGRAY);

        EndDrawing(); // End drawing
        for (auto& circle : CollissionEngine::getCircles()) {
            circle.update(GetFrameTime());
        }
    }

    // Close the window and OpenGL context
    CloseWindow();

    return 0;
}

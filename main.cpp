#include <raylib.h>

int main() {
    // Window dimensions
    const int screenWidth = 800;
    const int screenHeight = 600;

    // Initialize the window
    InitWindow(screenWidth, screenHeight, "Basic Raylib Circle Example");

    // Set the target FPS
    SetTargetFPS(60);

    while (!WindowShouldClose()) { // Check if the window should close
        // Update logic here (if any)

        // Start drawing
        BeginDrawing();
        ClearBackground(RAYWHITE); // Clear the background with white color

        // Draw a circle in the center of the window
        DrawCircle(screenWidth / 2, screenHeight / 2, 50, RED); // (x, y, radius, color)

        // Draw other shapes or text here if needed
        DrawText("Press ESC to close", 10, 10, 20, DARKGRAY);

        EndDrawing(); // End drawing
    }

    // Close the window and OpenGL context
    CloseWindow();

    return 0;
}

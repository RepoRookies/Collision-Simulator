#define RAYGUI_IMPLEMENTATION
#pragma warning(disable : 4996)
#include <raylib.h>
#include <raygui.h>

#include "src/CollissionEngine/CollissionEngine.h"
#include "src/Circle/Circle.h"
#include <vector>

int main() {
    // Window Dimensions
    const int window_width = 800;
    const int window_height = 600;

    const int nof_circles = 5;

    const int position_offset = 10;
    const float spray_wait = .25f;
	float time = 0;

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

    InitWindow(window_width, window_height, "DEEZ NUTS");
    CollissionEngine::load();

    /**************** Test Cases ****************/
    TestCase* test_case1
        = (new TestCase())
        ->SetNumBalls(1)
        ->SetRestitution(.8)
        ->SetGravity(6)
        ->SetBallVelocity(Vec2D(0, 0))
        ->SetSpawnPoint(Vec2D(window_width / 2, window_height / 2));

    TestCase* test_case2
        = (new TestCase())
        ->SetNumBalls(2)
        ->SetRestitution(1.)
        ->SetGravity(0)
        ->SetBallVelocity(Vec2D(750, 540))
        ->SetSpawnPoint(Vec2D(window_width / 2, window_height / 2));

    TestCase* test_case3
        = (new TestCase())
        ->SetNumBalls(3)
        ->SetRestitution(.9)
        ->SetGravity(5)
        ->SetBallVelocity(Vec2D(100, 0))
        ->SetSpawnPoint(Vec2D(window_width / 2, window_height / 2));

	TestCase::SetCurrTestCase(test_case1);




    // Set the Target FPS
    //SetTargetFPS(60);
    bool showMessageBox = false;

    while (!WindowShouldClose()) { 

        if (TestCase::GetCurrTestCase() == nullptr) {
			TestCase::SetCurrTestCase(new TestCase());
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);

   //     if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			//vec2 mouse(GetMousePosition());
			//CollissionEngine::addCircle(Circle(Vec2D(mouse.x, mouse.y), RADIUS, Vec2D(100, 0)));
   //     }
		time += GetFrameTime();
        if (CollissionEngine::getCircles().size() < TestCase::GetNumBalls() && time > spray_wait) {
			CollissionEngine::addCircle(Circle(TestCase::GetSpawnPoint(), RADIUS, TestCase::GetBallVelocity()));
            time = 0;
        }

        // Draw Circles
        for (auto& circle : CollissionEngine::getCircles()) {
            circle.drawGfx();
        }

        //Overall Collision Handler
		CollissionEngine::Simulate(Core::SimType::NO_HASH);

        // Draw other shapes or text here if needed
        DrawText(TextFormat("FPS : %d", GetFPS()), 10, 10, 20, DARKGRAY);

		Rectangle test_case_rec1 = { WIDTH - 120, 10, 100, 40 };
		Rectangle test_case_rec2 = { WIDTH - 120, 60, 100, 40 };
		Rectangle test_case_rec3 = { WIDTH - 120, 110, 100, 40 };
        if (GuiButton(test_case_rec1, "Test 1")) TestCase::SetCurrTestCase(test_case1);
        if (GuiButton(test_case_rec2, "Test 2")) TestCase::SetCurrTestCase(test_case2);
        if (GuiButton(test_case_rec3, "Test 3")) TestCase::SetCurrTestCase(test_case3);
        

        EndDrawing(); // End drawing
        for (auto& circle : CollissionEngine::getCircles()) {
            circle.update(GetFrameTime());
        }
    }

    CollissionEngine::unload();
    // Close the window and OpenGL context
    CloseWindow();

    return 0;
}

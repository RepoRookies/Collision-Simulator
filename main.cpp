#define RAYGUI_IMPLEMENTATION
#pragma warning(disable : 4996)

#include <raylib.h>
#include <raygui.h>
#include <vector>

#include "src/CollissionEngine/CollissionEngine.h"
#include "src/Circle/Circle.h"

int main() {
    const int window_width = 800;
    const int window_height = 600;
 
    const float spray_wait = .02f;
	float time = 0;

    InitWindow(window_width, window_height, "Collision-Simulator by Team Processor Heaters");
    CollissionEngine::load();

    /* ************** * Test Cases * ************** */

    TestCase* test_case1
        = (new TestCase())
        ->SetNumBalls(1)
        ->SetRestitution(.75)
        ->SetGravity(1000)
        ->SetBallVelocity(Vec2D(0, 0))
        ->SetSpawnPoint(Vec2D(window_width / 2, window_height / 2))
        ->SetRadius(40)
        ->SetIsSpacialHash(true);

    TestCase* test_case2
        = (new TestCase())
        ->SetNumBalls(25)
        ->SetRestitution(1.)
        ->SetGravity(0)
        ->SetBallVelocity(Vec2D(750, 540))
        ->SetSpawnPoint(Vec2D(window_width / 2, window_height / 2));

    TestCase* test_case3
        = (new TestCase())
        ->SetNumBalls(1500)
        ->SetRestitution(.9)
        ->SetGravity(1000)
        ->SetBallVelocity(Vec2D(100, 0))
        ->SetRadius(8)
        ->SetSpawnPoint(Vec2D(window_width / 2, window_height / 2))
        ->SetIsSpacialHash(true)
        ->SetIsParallel(true);

	TestCase::SetCurrTestCase(test_case1);

    SetTargetFPS(120);
    bool showMessageBox = false;

    while (!WindowShouldClose()) { 
        if (TestCase::GetCurrTestCase() == nullptr) {
			TestCase::SetCurrTestCase(new TestCase());
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);
		//CollissionEngine::drawGrid();
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
        if (TestCase::IsSpacialHash())         {
            if (!TestCase::IsParallel())
                CollissionEngine::Simulate(Core::SimType::HASH);
            else
				CollissionEngine::Simulate(Core::SimType::HASH_PARALLEL);
        } else {
			CollissionEngine::Simulate(Core::SimType::NO_HASH);
        }

        DrawText(TextFormat("FPS : %d", GetFPS()), 10, 10, 20, DARKGRAY);

		Rectangle test_case_rec1 = { WIDTH - 120, 10, 100, 40 };
		Rectangle test_case_rec2 = { WIDTH - 120, 60, 100, 40 };
		Rectangle test_case_rec3 = { WIDTH - 120, 110, 100, 40 };

        if (GuiButton(test_case_rec1, "Test 1")) TestCase::SetCurrTestCase(test_case1);
        if (GuiButton(test_case_rec2, "Test 2")) TestCase::SetCurrTestCase(test_case2);
        if (GuiButton(test_case_rec3, "Test 3")) TestCase::SetCurrTestCase(test_case3);

        EndDrawing();
        for (auto& circle : CollissionEngine::getCircles()) {
            circle.update(GetFrameTime());
        }
    }
    CollissionEngine::unload();
    CloseWindow();
    return 0;
}

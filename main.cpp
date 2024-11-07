#define RAYGUI_IMPLEMENTATION
#pragma warning(disable : 4996)

#include <raylib.h>
#include <raygui.h>
#include <vector>
#include <mpi.h>

#include "src/CollissionEngine/CollissionEngine.h"
#include "src/Circle/Circle.h"

int main(int argc, char** argv) {
    int rank, size;

    std::vector<i32> gridIds, localGridIds;
    i32 nofGrids, localNofGrids;

    MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int window_width = 800;
    const int window_height = 600;

    const float spray_wait = .02f;
    float time = 0;

    if (rank == 0) {
        InitWindow(window_width, window_height, "Collision-Simulator by Team Processor Heaters");
        CollissionEngine::load();
    }

    /* ************** * Test Cases * ************** */

    TestCase* test_case1
        = (new TestCase())
        ->SetNumBalls(1)
        ->SetRestitution(.75)
        ->SetGravity(1000)
        ->SetBallVelocity(Vec2D(0, 0))
        ->SetSpawnPoint(Vec2D(window_width / 2, window_height / 2))
        ->SetRadius(40)
        ->SetIsSpacialHash(true)
        ->SetIsMPI(true);

    TestCase* test_case2
        = (new TestCase())
        ->SetNumBalls(25)
        ->SetRestitution(1.)
        ->SetGravity(0)
        ->SetBallVelocity(Vec2D(750, 540))
        ->SetSpawnPoint(Vec2D(window_width / 2, window_height / 2))
        ->SetIsMPI(true);

    TestCase* test_case3
        = (new TestCase())
        ->SetNumBalls(1500)
        ->SetRestitution(.9)
        ->SetGravity(1000)
        ->SetBallVelocity(Vec2D(100, 0))
        ->SetRadius(8)
        ->SetSpawnPoint(Vec2D(window_width / 2, window_height / 2))
        ->SetIsSpacialHash(true)
        ->SetIsParallel(true)
        ->SetIsMPI(true);

    TestCase::SetCurrTestCase(test_case1);
       
    if (rank == 0) {
        nofGrids = CollissionEngine::getNumGrids();
    }
    MPI_Bcast(&nofGrids, 1, MPI_INT, MPI_ROOT, MPI_COMM_WORLD);
    localNofGrids = nofGrids / size;
    for (size_t i = 0; i < nofGrids; i++) {
        gridIds.push_back(storeCircleId(i, nofGrids));
    }

    localGridIds.resize(localNofGrids);
    MPI_Scatter(
        gridIds.data(),
        localNofGrids,
        MPI_INT,
        localGridIds.data(),
        localNofGrids,
        MPI_INT,
        MPI_ROOT,
        MPI_COMM_WORLD
    );

    std::cout << "Process: " << rank << " -> [" << localNofGrids << "]: ";
    for (auto id : localGridIds) {
        std::cout << id << " ";
    }
    std::cout << "\n" << std::endl;

    MPI_Gather(
        localGridIds.data(),
        localNofGrids,
        MPI_INT,
        gridIds.data(),
        localNofGrids,
        MPI_INT,
        MPI_ROOT,
        MPI_COMM_WORLD
    );
 
    if (rank == 0) {
        std::cout << "Process: " << rank << " collected Grid Data: ";
        for (auto id : gridIds) {
            std::cout << id << " ";
        }
        std::cout << "\n" << std::endl;

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
            if (TestCase::IsSpacialHash()) {
                if (!TestCase::IsParallel())
                    CollissionEngine::Simulate(Core::SimType::HASH,size,rank);
				else if (!TestCase::IsMPI())
                    CollissionEngine::Simulate(Core::SimType::HASH_PARALLEL,size,rank);
                else
					CollissionEngine::Simulate(Core::SimType::HASH_PARALLEL_MPI,size,rank);
            }
            else {
                CollissionEngine::Simulate(Core::SimType::NO_HASH,size,rank);
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
    }
    else {
        CollissionEngine::Simulate(Core::SimType::HASH_PARALLEL_MPI, size, rank);
    }

    MPI_Finalize();
    return 0;
}

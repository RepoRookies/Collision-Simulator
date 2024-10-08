#include "CollissionEngine.h"
#include "../TestCases/TestCase.h"
#include <omp.h>

std::vector <Circle> CollissionEngine::circles;
std::vector<std::vector<std::vector<i32>>> CollissionEngine::colliders_per_cell;
std::vector<std::vector<i32>> CollissionEngine::num_colliders_per_cell;
i32 CollissionEngine::cellsX = 0;
i32 CollissionEngine::cellsY = 0;
bool CollissionEngine::is_initialized = false;

void CollissionEngine::load() {
	Image image = LoadImage("assets/Circle.png");
	ImageResize(
		&image,
		image.width / Circle::imageRescaleFactor,
		image.height / Circle::imageRescaleFactor
	);
	Circle::circle_texture = LoadTextureFromImage(image);
	UnloadImage(image);
	is_initialized = true;
}

void CollissionEngine::InitSpacialHash() {
	omp_set_num_threads(NUM_THREADS == 0 ? omp_get_num_procs(): NUM_THREADS);

	//Spatial Hash Initialization
	cellsX = GetScreenWidth() / (2*RADIUS);
	cellsY = GetScreenHeight() / (2*RADIUS);

	auto cellVector = std::vector<i32>(MAX_COLLIDERS_PER_CELL, 0);
	auto cellXVector = std::vector<std::vector<i32>>(cellsY, cellVector);
	colliders_per_cell = std::vector<std::vector<std::vector<i32>>>(cellsX, cellXVector);

	auto num_cellVector = std::vector<i32>(cellsY, 0);
	num_colliders_per_cell = std::vector<std::vector<i32>>(cellsX, num_cellVector);
	is_initialized = true;
}

void CollissionEngine::resizeBall()
{
	Image image = LoadImage("assets/Circle.png");
	TraceLog(LOG_INFO, "Ball Radius : %f");
	ImageResize(
		&image,
		image.width * (TestCase::GetRadius()/10) / Circle::imageRescaleFactor,
		image.height * (TestCase::GetRadius()/10) / Circle::imageRescaleFactor
	);
	Circle::circle_texture = LoadTextureFromImage(image);
	UnloadImage(image);
}

void CollissionEngine::simulate_no_hash() {
	for (size_t i = 0; i < COLLISION_CLOCK_RATE; i++) {
		for (auto& first : circles) {
			for (auto& second : circles) {
				if (&first == &second) 
					continue;

				Circle::handleCollision(first, second);
			}
		}
	}
}

void CollissionEngine::GenerateSpatialGrid()
{
	for (i32 x = 0; x < cellsX; x++) {
		for (i32 y = 0; y < cellsY; y++) {
			num_colliders_per_cell[x][y] = 0;
		}
	}
	for (i32 col_id = 0; col_id < circles.size(); col_id++) {
		Circle& circle= circles[col_id];
		Vec2D upperLeftCorner_screen(0, 0);
		Vec2D circle_pos = circle.center;
		circle_pos = circle_pos - upperLeftCorner_screen;
		// col_pos.pri32();
		Vec2D circle_size= Vec2D(circle.radius, circle.radius);
		Vec2D neg_circle_size= Vec2D(-circle_size.x_comp, -circle_size.y_comp);

		// NOTE: Bharath: This method isn't perfect at all, and I don't have any great Ideas, But having most colliders
		//                Less than GRID_CELL_SIZE will be really good for performance unless GRID_CELL_SIZE is too big
		Vec2D col_size_indices_start = GetColIndicesFloor(neg_circle_size);
		Vec2D col_size_indices_stop = GetColIndicesCeil(circle_size);
		for (i32 i = col_size_indices_start.x_comp; i <= col_size_indices_stop.x_comp; i++) {
			for (i32 j = col_size_indices_start.y_comp; j <= col_size_indices_stop.y_comp; j++) {
				Vec2D pos_ind = GetColIndicesFloor(circle_pos);
				pos_ind = pos_ind + Vec2D(i,j); 
				if (pos_ind.x_comp >= cellsX || pos_ind.y_comp >= cellsY)
					continue;
				if (pos_ind.x_comp < 0 || pos_ind.y_comp < 0)
					continue;
				if (num_colliders_per_cell[pos_ind.x_comp][pos_ind.y_comp] + 1 >= MAX_COLLIDERS_PER_CELL)
					break;
				i32 cur_num_col_cell = num_colliders_per_cell[pos_ind.x_comp][pos_ind.y_comp]++;
				colliders_per_cell[pos_ind.x_comp][pos_ind.y_comp][cur_num_col_cell] = col_id;
			}
		}
	}
}


void CollissionEngine::simulate_hash() {
	for (size_t i = 0; i < COLLISION_CLOCK_RATE; i++) {
		GenerateSpatialGrid();
		SolveCollissionsHash();
	}
}

void CollissionEngine::simulate_hash_parallel() {

	for (size_t i = 0; i < COLLISION_CLOCK_RATE; i++) {
		GenerateSpatialGrid();
		SolveCollissionsHashParallel();
	}
}

void CollissionEngine::SolveCollissionsHash()
{
	if (!is_initialized)
		return;
	for (i32 x = 0; x < cellsX; x++) {
		for (i32 y = 0; y < cellsY; y++) {
			SolveCollissionsForCellHash(x, y);
		}
	}
}

void CollissionEngine::SolveCollissionsHashParallel()
{
	if (!is_initialized)
		return;
	#pragma omp parallel for collapse(2) schedule(static, cellsY) shared(colliders_per_cell, num_colliders_per_cell)
	for (i32 x = 0; x < cellsX; x++) {
		for (i32 y = 0; y < cellsY; y++) {
			SolveCollissionsForCellHash(x, y);
		}
	}
}

void CollissionEngine::SolveCollissionsForCellHash(i32 x, i32 y)
{
	for (i32 i = 0; i < num_colliders_per_cell[x][y]; i++) {
		Circle& curr_circle = circles[colliders_per_cell[x][y][i]];
		for (i32 j = 0; j < num_colliders_per_cell[x][y]; j++) {
			if (i == j)
				continue;
			Circle& other_circle = circles[colliders_per_cell[x][y][j]];
			Circle::handleCollision(curr_circle, other_circle);
		}
	}
}

void CollissionEngine::unload()
{
	UnloadTexture(Circle::circle_texture);
}

void CollissionEngine::drawGrid()
{

	for (i32 gridX = 0; gridX <= cellsX; gridX += 1) {
		DrawLine( gridX * 2*RADIUS, 0,
			+ gridX * 2*RADIUS,  GetScreenHeight(),
			GREEN);
	}

	for (i32 gridY = 0; gridY <= cellsY; gridY += 1) {
		DrawLine(0, gridY * 2*RADIUS,
			GetScreenWidth(),  gridY * 2*RADIUS,
			GREEN);
	}

	for (i32 x = 0; x < cellsX; x++)
		for (i32 y = 0; y < cellsY; y++)
			for (i32 i = 0; i < num_colliders_per_cell[x][y]; i++)
				DrawText(TextFormat("%d", colliders_per_cell[x][y][i]),  x * 2 *RADIUS + i * 10,  y * 2*RADIUS, 10, RED);
}


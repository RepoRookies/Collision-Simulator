#pragma once
#include <vector>
#include "../Circle/Circle.h"

#define COLLISION_CLOCK_RATE 8
#define MAX_COLLIDERS_PER_CELL 100
#define NUM_THREADS 4

/**
* Collission Engine Class
* Collission Engine does not create or destroy collider
* It just provides abstractions to handle collissions
*/
namespace Core {
	enum class SimType {
		NO_HASH,
		HASH,
		HASH_PARALLEL
	};
}

class CollissionEngine {
private:
	static i32 cellsX;
	static i32 cellsY;
	static bool is_initialized;

	static std::vector <Circle> circles;
	static void GenerateSpatialGrid();
	static void simulate_no_hash();
	static void simulate_hash();
	static void simulate_hash_parallel();

	/******** Spatial Hashing Specific ********/
	static std::vector<std::vector<std::vector<i32>>> colliders_per_cell;
	static std::vector<std::vector<i32>> num_colliders_per_cell;
	static void SolveCollissionsHash();
	static void SolveCollissionsHashParallel();
	static void SolveCollissionsForCellHash(i32 x, i32 y);
	static inline Vec2D GetColIndicesFloor(Vec2D col_pos) {
		return  Vec2D(floor(col_pos.x_comp / (2*RADIUS)), floor(col_pos.y_comp / (2*RADIUS)));
	}
	static inline Vec2D GetColIndicesCeil(Vec2D col_pos) {
		return  Vec2D(ceil(col_pos.x_comp / (2*RADIUS)), ceil(col_pos.y_comp / (2*RADIUS)));
	}

public:
	static void load();
	static void InitSpacialHash();
	static void resizeBall();
	static void unload();
	inline static void Simulate(Core::SimType sim_type) {
		if (!is_initialized)
			return;
		switch (sim_type) {
			case Core::SimType::NO_HASH: {
				simulate_no_hash();
				return;
			} case Core::SimType::HASH: {
				simulate_hash();
				return;
			} case Core::SimType::HASH_PARALLEL: {
				simulate_hash_parallel();
				return;
			}
		}
	}

	//********* Inline Functions *********//
	static inline void addCircle(Circle circle) {
		circle.setCircleId(circles.size());
		circles.push_back(circle);
	}
	static inline std::vector<Circle>& getCircles() {
		return circles;
	}
	static void drawGrid();
};

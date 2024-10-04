#pragma once
#include <vector>
#include "../Circle/Circle.h"

#define COLLISION_CLOCK_RATE 8

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
	static std::vector <Circle> circles;
	static void simulate_no_hash();
	static void simulate_hash();
	static void simulate_hash_parallel();
	
public:
	static void load();
	static void unload();
	inline static void Simulate(Core::SimType sim_type) {
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
	static inline void addCircle(const Circle& circle) {
		circles.push_back(circle);
	}
	static inline std::vector<Circle>& getCircles() {
		return circles;
	}
};

#include "CollissionEngine.h"

std::vector <Circle> CollissionEngine::circles;

void CollissionEngine::init() {
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

void CollissionEngine::simulate_hash() {
}

void CollissionEngine::simulate_hash_parallel() {
}


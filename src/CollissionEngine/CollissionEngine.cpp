#include "CollissionEngine.h"

std::vector<Circle> CollissionEngine::circles;

void CollissionEngine::simulate_no_hash()
{
	for (auto& first: circles){
		for (auto& second : circles) {
			if (&first == &second) continue;
			if (Circle::isCollision(first, second)) {
				Circle::handleCollision(first, second, GetScreenWidth(), GetScreenHeight());
			}
		}
	}
}

void CollissionEngine::simulate_hash()
{
}

void CollissionEngine::simulate_hash_parallel()
{
}

void CollissionEngine::init()
{
}

#include "CollissionEngine.h"
#include "../TestCases/TestCase.h"

std::vector <Circle> CollissionEngine::circles;

void CollissionEngine::load() {
	Image image = LoadImage("assets/Circle.png");
	ImageResize(
		&image,
		image.width / Circle::imageRescaleFactor,
		image.height / Circle::imageRescaleFactor
	);
	Circle::circle_texture = LoadTextureFromImage(image);
	UnloadImage(image);
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

void CollissionEngine::simulate_hash() {
}

void CollissionEngine::simulate_hash_parallel() {
}

void CollissionEngine::unload()
{
	UnloadTexture(Circle::circle_texture);
}


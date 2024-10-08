#pragma once

#include "../Core/drawable.hpp"
#include "../Vector/Vector.h"
#include "../TestCases/TestCase.h"
#include <raylib.h>
#include <utility>

#define RADIUS TestCase::GetRadius()
#define WIDTH GetScreenWidth()
#define HEIGHT GetScreenHeight()
#define RESTITUTION TestCase::GetRestitution()

class Circle : Core::Drawable {
private:
	static bool texture_loaded;
	i32 circle_id = -1;
public:
	static Texture2D circle_texture;
	static f32 imageRescaleFactor;
	void init() override;
	void update(f32 delta) override;
	void drawGfx() override;

	double radius;		// Radius
	Vec2D center;		// Center (x, y) Coordinates
	Vec2D velocity;		// 2D Velocity Vector
	double gravity;		// Individual Gravity Component
	Color color;		// Circle Color

	// Circle Constructor
	Circle(
		Vec2D center = Vec2D(WIDTH / 2, HEIGHT / 2),
		double radius = RADIUS,
		Vec2D velocity = Vec2D(0, 0),
		double gravity = 0, 
		Color color = RED
	);

	inline void setCircleId(i32 id) {
		circle_id = id;
	}

	inline void handleGravity(int limit) {
		gravity = (center.y_comp + radius < limit) ? TestCase::GetGravity() : 0;
		velocity.incVec2D(0, gravity * GetFrameTime());
	}
	inline void moveCircle(f32 delta) {
		center.x_comp += velocity.x_comp * GetFrameTime();
		center.y_comp += velocity.y_comp * GetFrameTime();
	}

	void handleWindowBounds();

	static double getCenterDistance(const Circle& first, const Circle& second);
	static double getOverlapDistance(const Circle& first, const Circle& second);
	static bool isCollision(const Circle& first, const Circle& second);

	static Vec2D getAxisVec2(const Circle& first, const Circle& second);
	static std::pair <Vec2D, Vec2D> getIdealCenters(const Circle& first, const Circle& second);
	static void setPostCollision(Circle& first, Circle& second);

	static void handleCollision(Circle& first, Circle& second);
};

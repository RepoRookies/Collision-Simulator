#pragma once

#include "../Vector/Vector.h"
#include <raylib.h>
#include <utility>
#include "../Core/drawable.hpp"

class Circle : Core::Drawable {
public:
	void init() override;
	void update(f32 delta) override;
	void drawGfx() override;

	double radius;		// Radius
	Vec2D center;		// Center (x, y) Coordinates
	Vec2D velocity;		// 2D Velocity Vector
	double gravity;		// Individual Gravity Component
	Color color;		// Circle Color

	// Circle Constructor
	Circle(Vec2D center = Vec2D(0, 0), double radius = 100, Vec2D velocity = Vec2D(0, 0), double gravity = 0, Color color = GREEN);

	// Gravity Manipulation
	void handleGravity(int limit);

	// Move Circle using Velocity Vector
	void moveCircle(f32 delta);

	// Distance between Centers of 2 Circles
	static double getCenterDistance(const Circle& first, const Circle& second);

	// Overlapping Distance between 2 Circles
	static double getOverlapDistance(const Circle& first, const Circle& second);
	
	// Check for Collision Between 2 Circles
	static bool isCollision(const Circle& first, const Circle& second);

	// Get Unit Vector (Axis of Collision)
	static Vec2D getAxisVec2(const Circle& first, const Circle& second);

	// Ideal Centers Post Collision
	static std::pair <Vec2D, Vec2D> getIdealCenters(const Circle& first, const Circle& second);

	// Center Updates Post Collision
	static void setPostCollision(Circle& first, Circle& second);

	// Check & Handle Window Bounds
	void handleWindowBounds(int window_width, int window_height, double restitution = 1);

	// Handles Overall Collision between 2 Circles
	static void handleCollision(Circle& first, Circle& second, int window_width, int window_height, double restitution = 1);
};

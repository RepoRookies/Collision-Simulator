#include "Circle.h"

#include <cmath>
#include <utility>
#include <raylib.h>

// Constructors Implementation
Circle::Circle(Vec2D center, double radius, Vec2D velocity, double gravity, Color color) : 
	center(center), radius(radius), velocity(velocity), gravity(gravity), color(color) 
{};

// Gravity Manipulation
void Circle::handleGravity(int limit) {
	gravity = (center.y_comp + radius < limit) ? 500 * GetFrameTime() : 0;
	velocity.incVec2D(0, gravity);
}

// Move Circle Implementation
void Circle::moveCircle() {
	center.x_comp += velocity.x_comp * GetFrameTime();
	center.y_comp += velocity.y_comp * GetFrameTime();
}

// Collision Function Implementations...
double Circle::getCenterDistance(const Circle& first, const Circle& second) {
	double dx = first.center.x_comp - second.center.x_comp;
	double dy = first.center.y_comp - second.center.y_comp;

	return std::sqrt(dx * dx + dy * dy);
}

double Circle::getOverlapDistance(const Circle& first, const Circle& second) {
	return first.radius + second.radius - Circle::getCenterDistance(first, second);
}

bool Circle::isCollision(const Circle& first, const Circle& second) {
	return Circle::getOverlapDistance(first, second) > 0;
}

// Get Axis of Collision
Vec2D Circle::getAxisVec2(const Circle& first, const Circle& second) {
	double dx = first.center.x_comp - second.center.x_comp;
	double dy = first.center.y_comp - second.center.y_comp;

	Vec2D axis_of_collision(dx, dy);
	return axis_of_collision.normalize();
}

// Get Ideal Centers Post Collision
std::pair <Vec2D, Vec2D> Circle::getIdealCenters(const Circle& first, const Circle& second) {
	double offset = Circle::getOverlapDistance(first, second) / 2;

	Vec2D unit_vec2 = Circle::getAxisVec2(first, second);

	Vec2D first_center(first.center.x_comp + offset * unit_vec2.x_comp, first.center.y_comp + offset * unit_vec2.y_comp);
	Vec2D second_center(second.center.x_comp - offset * unit_vec2.x_comp, second.center.y_comp - offset * unit_vec2.y_comp);

	return { first_center, second_center };
}

// Center Updates Post Collision
void Circle::setPostCollision(Circle& first,Circle& second) {
	auto points = Circle::getIdealCenters(first, second);
	first.center = points.first;
	second.center = points.second;
}

// Window Bounds Implementation
void Circle::handleWindowBounds(int window_width, int window_height, double restitution) {
	if (0 >= center.x_comp - radius || center.x_comp + radius >= window_width) {
		velocity.x_comp *= -restitution;
	}
	if (0 >= center.y_comp - radius || center.y_comp + radius >= window_height) {
		velocity.y_comp *= -restitution;
	}
}

// Overall Collision Handler Implementation
void Circle::handleCollision(Circle& first, Circle& second, int window_width, int window_height, double restitution) {
	bool collision = Circle::isCollision(first, second);
	first.color = second.color = collision ? RED : GREEN;

	if (collision) {
		Circle::setPostCollision(first, second);

		auto unit_vec2 = Circle::getAxisVec2(first, second);
		auto relative_velocity_scalar = (first.velocity - second.velocity) * unit_vec2;

		first.velocity.incVec2D(unit_vec2.multiplyScalar(-(relative_velocity_scalar)));
		first.velocity.incVec2D(unit_vec2.multiplyScalar(relative_velocity_scalar));
	}
}

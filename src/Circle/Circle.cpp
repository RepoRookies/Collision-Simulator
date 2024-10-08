#include "Circle.h"

#include <cmath>
#include <utility>
#include <algorithm>
#include <raylib.h>
#include <direct.h>

// Static Texture2D Initialization
Texture2D Circle::circle_texture;
bool Circle::texture_loaded = false;
float Circle::imageRescaleFactor = 1.5;

void Circle::init() {
	// Initialization Code Here...
}

void Circle::update(f32 delta) {
	// Initialization Code Here...
	handleGravity(HEIGHT);
	moveCircle(delta);
	handleWindowBounds();
}

void Circle::drawGfx() {
	if (circle_texture.width == 0 || circle_texture.height == 0) {
		//TraceLog(LOG_ERROR, "Texture loading failed!");
	}
	Vector2 pos = { center.x_comp - circle_texture.width * radius/40,center.y_comp - circle_texture.height * radius/40 };
	DrawTextureEx(circle_texture, pos, 0, (float)radius / 20.f, color); //- circle_texture.width/2,center.y_comp - circle_texture.height/2,color);
	//For Debug Purposes
	//DrawCircleLines(center.x_comp, center.y_comp, radius, color);
}

// Constructors Implementation
Circle::Circle(Vec2D center, double radius, Vec2D velocity, double gravity, Color color) : 
	center(center), radius(radius), velocity(velocity), gravity(gravity), color(color)
{};


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
// Ideal -> Centers of Circles Post Collision
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
void Circle::handleWindowBounds() {
	if (0 >= center.x_comp - radius) {
		center.x_comp = radius;
		velocity.x_comp *= -RESTITUTION;
	} else if (center.x_comp + radius >= WIDTH) {
		center.x_comp = WIDTH - radius;
		velocity.x_comp *= -RESTITUTION;
	}
	if (0 >= center.y_comp - radius) {
		center.y_comp = radius;
		velocity.y_comp *= -RESTITUTION;
	} else if (center.y_comp + radius >= HEIGHT) {
		center.y_comp = HEIGHT - radius;
		velocity.y_comp *= -RESTITUTION;
	}
}

// Overall Collision Handler Implementation
void Circle::handleCollision(Circle& first, Circle& second) {
	bool collision = Circle::isCollision(first, second);

	if (collision) {
		first.handleWindowBounds();
		second.handleWindowBounds();

		Circle::setPostCollision(first, second);

		auto unit_vec2 = Circle::getAxisVec2(first, second);
		auto relative_velocity_scalar = (first.velocity - second.velocity) * unit_vec2;

		first.velocity.incVec2D(unit_vec2.multiplyScalar(-(relative_velocity_scalar)));
		second.velocity.incVec2D(unit_vec2.multiplyScalar(relative_velocity_scalar));

		first.handleWindowBounds();
		second.handleWindowBounds();
	}
}

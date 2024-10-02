#include "Vector.h"

#include <cmath>

// Vec2D Constructor Definition
Vec2D::Vec2D(double x, double y) : x_comp(x), y_comp(y) {};

// Increment Vector Components
void Vec2D::incVec2D(double x, double y) {
    x_comp += x;
    y_comp += y;
}

void Vec2D::incVec2D(const Vec2D& vector) {
    x_comp += vector.x_comp;
    y_comp += vector.y_comp;
}

// Scalar Multiplication
Vec2D Vec2D::multiplyScalar(double scalar) {
    return Vec2D(x_comp * scalar, y_comp * scalar);
}

// Vector Addition
Vec2D Vec2D::operator+(const Vec2D& other) {
    return Vec2D(x_comp + other.x_comp, y_comp + other.y_comp);
}

// Vector Subtraction
Vec2D Vec2D::operator-(const Vec2D& other) {
    return Vec2D(x_comp - other.x_comp, y_comp - other.y_comp);
}

// Dot Product
double Vec2D::operator*(const Vec2D& other) {
    return x_comp * other.x_comp + y_comp * other.y_comp;
}

// Cross Product 
double Vec2D::operator%(const Vec2D& other) {
    return x_comp * other.y_comp - y_comp * other.x_comp;
}

// Magnitude
double Vec2D::magnitude() const {
    return std::sqrt(x_comp * x_comp + y_comp * y_comp);
}

// Normalization or Make Unit Vector
Vec2D Vec2D::normalize() const {
    double mag = magnitude();

    if (mag == 0) {
        return Vec2D(0, 0);
    }
    return Vec2D(x_comp / mag, y_comp / mag);
}

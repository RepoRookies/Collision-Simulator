#pragma once

class Vec2D {
public:
    // Components of 2D Vector
    double x_comp, y_comp;

    // Vector Constructor
    Vec2D(double x = 0, double y = 0);

    // Increment Vector Components
    void incVec2D(double x, double y);
    void incVec2D(const Vec2D& vector);

    // Scalar multiplication
    Vec2D multiplyScalar(double scalar);

    // Vector Addition
    Vec2D operator+(const Vec2D& other);

    // Vector Subtraction
    Vec2D operator-(const Vec2D& other);

    // Dot Product
    double operator*(const Vec2D& other);

    // Cross Product
    double operator%(const Vec2D& other);

    // Magnitude
    double magnitude() const;

    // Normalization or Make Unit Vector
    Vec2D normalize() const;
};

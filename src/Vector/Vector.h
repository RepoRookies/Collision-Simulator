#pragma once

class Vec2D {
public:
    double x_comp, y_comp;  // Components of 2D Vector
    
    Vec2D(double x = 0, double y = 0);  // Vector Constructor

    // Increment Vector Components
    inline void incVec2D(double x, double y) {
        x_comp += x;
        y_comp += y;
    }
    inline void incVec2D(const Vec2D& vector) {
        x_comp += vector.x_comp;
        y_comp += vector.y_comp;
    }

    Vec2D multiplyScalar(double scalar);    // Scalar Multiplication

    Vec2D operator+(const Vec2D& other);    // Vector Addition
    Vec2D operator-(const Vec2D& other);    // Vector Subtraction
    double operator*(const Vec2D& other);   // Dot Product
    double operator%(const Vec2D& other);   // Cross Product

    double magnitude() const;   // Magnitude
    Vec2D normalize() const;    // Normalization or Make Unit Vector
};

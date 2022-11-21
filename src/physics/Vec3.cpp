// Vec3.cpp
// Created: Wed 02 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------


#include "Vec3.hpp"
#include <math.h>


// Default Constructor
Vec3::Vec3(): x(0.0), y(0.0), z(0.0) {}

// Constructor
Vec3::Vec3(float x, float y, float z): x(x), y(y), z(z) {}

void Vec3::Add(const Vec3& v){
    x += v.x;
    y += v.y;
    z += v.z;
}

void Vec3::Sub(const Vec3& v){
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

void Vec3::Scale(const float n) {
    x *= n;
    y *= n;
    z *= n;
}

float Vec3::Magnitude() const {
    return sqrtf(x*x + y*y + z*z);
}

float Vec3::MagnitudeSquared() const {
    return (x*x + y*y  + z*z);
}

Vec3& Vec3::Normalize() {
    float length = Magnitude();
    if (length != 0.0) {
        x /= length;
        y /= length;
        z /= length;
    }
    return *this;
}

Vec3 Vec3::UnitVector() const {
    Vec3 result = Vec3(0, 0, 0);
    float length = Magnitude();
    if (length != 0.0) {
        result.x = x / length;
        result.y = y / length;
        result.z = z / length;
    }
    return result;
}

float Vec3::Dot(const Vec3& v) const {
    return (x * v.x) + (y * v.y) + (z * v.z);
}

// ---- Operator Overloading ----

// Overload the assignment operator
Vec3& Vec3::operator = (const Vec3& v) {
    x = v.x;
    y = v.y;
    z = v.z;
    
    return *this;
}

// Overload the equality operator
bool Vec3::operator == (const Vec3& v) const {
    return x == v.x && y == v.y && z == v.z;
}

// Overload the not-equality operator
bool Vec3::operator != (const Vec3& v) const {
    return !(*this == v);
}

// Overload the addition functionality
Vec3 Vec3::operator + (const Vec3& v) const {
    Vec3 result;
    result.x = x + v.x;
    result.y = y + v.y;
    result.z = z + v.z;
    
    return result;
}

// Overload the subtraction functionality
Vec3 Vec3::operator - (const Vec3& v) const {
    Vec3 result;
    result.x = x - v.x;
    result.y = y - v.y;
    result.z = z - v.z;
    
    return result;
}

// Overload the multiplication functionality
Vec3 Vec3::operator * (const float n) const {
    Vec3 result;
    result.x = x * n;
    result.y = y * n;
    result.z = z * n;
    
    return result;
}

// Overload the division functionality
Vec3 Vec3::operator / (const float n) const {
    Vec3 result;
    result.x = x / n;
    result.y = y / n;
    result.z = z / n;
    
    return result;
}

// Overload the negation functionality
Vec3 Vec3::operator - () {
    Vec3 result;
    result.x = x * -1;
    result.y = y * -1;
    result.z = z * -1;
    
    return result;
}

// Overload +=
Vec3& Vec3::operator += (const Vec3& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    
    return *this;
}

// Overload -=
Vec3& Vec3::operator -= (const Vec3& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    
    return *this;
}

// Overload *=
Vec3& Vec3::operator *= (const float n) {
    x *= n;
    y *= n;
    z *= n;
    
    return *this;
}

// Overload /=
Vec3& Vec3::operator /= (const float n){
    x /= n;
    y /= n;
    z /= n;
    
    return *this;
}




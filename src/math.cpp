//
//  math.cpp
//  engine
//
//  Created by Seymour Pashayev on 11/2/22.
//

#include "math.hpp"
#include <cmath>

// 2 Dimensional Vector
struct Vec2 {
    
    // ---- Variables ----
    float x;
    float y;
    
    // ---- Init ----
    
    // Constructor
    Vec2(float x, float y): x(x), y(y) {};
    
    // Destructor
    ~Vec2() = default;
    
    // ---- Dynamic Vector Operations ----
    
    // Magnitude
    float getMagnitude(){
        return sqrt(x*x + y*y);
    }
    
    // Normalize
    void normalize() {
        float mag = this->getMagnitude();
        
        this->x /= mag;
        this->y /= mag;
    }
    
    // Perpendicular Vector
    Vec2* perperdicular() {
        Vec2* result = new Vec2(this->y, -this->x);
        
        return result;
    }
    
    // Vector Addition
    void add(Vec2 vec) {
        this->x += vec.x;
        this->y += vec.y;
    }
    
    // Vector Subtraction
    void sub(Vec2 vec){
        this->x -= vec.x;
        this->y -= vec.y;
    }
    
    // Vector Scaling
    void scale(float val){
        this->x *= val;
        this->y *= val;
    }
    
    // Dot Product
    float dotProduct(Vec2 v){
        return (this->x * v.x) + (this->y * v.y);
    }
    
    // ---- Static Vector Operations ----
    
    // Static Vector Addition
    static Vec2* add(Vec2 v1, Vec2 v2){
        Vec2* result = new Vec2(0,0);
        result->x = v1.x + v2.x;
        result->y = v1.y + v2.y;
        
        return result;
    }
    
    // Static Vector Subtraction
    static Vec2* sub(Vec2 v1, Vec2 v2){
        Vec2* result = new Vec2(0,0);
        result->x = v1.x - v2.x;
        result->y = v1.y - v2.y;
        
        return result;
    }
    
    // Static Dot Product
    static float dotProduct(Vec2 v1, Vec2 v2){
        return (v1.x * v2.x) + (v1.y * v2.y);
    }
    
}; //ENDOF: Vec2


// 3-Dimensional Vector
struct Vec3 {
    
    // ---- Variables ----
    float x;
    float y;
    float z;
    
    // ---- Init -----
    
    // Constructor
    Vec3(float x, float y, float z): x(x), y(y), z(z) {};

    // Destructor
    ~Vec3() = default;
    
    // ---- Dynamic Vector Operations ----
    
    // Magnitude
    float getMagnitude(){
        return sqrt(x*x + y*y + z*z);
    }
    
    // Normalize
    void normalize() {
        float mag = this->getMagnitude();
        
        this->x /= mag;
        this->y /= mag;
        this->z /= mag;
    }
    
    // Vector Addition
    void add(Vec3 vec) {
        this->x += vec.x;
        this->y += vec.y;
        this->z += vec.z;
    }
    
    // Vector Subtraction
    void sub(Vec3 vec) {
        this->x -= vec.x;
        this->y -= vec.y;
        this->z -= vec.z;
    }
    
    // Vector Scaling
    void scale(float val){
        this->x *= val;
        this->y *= val;
        this->z *= val;
    }
    
    // Dot (Scalar) Product - The output of the operation is NOT a vector
    // (How aligned are the vectors from 1 being aligned to -1 being not aligned; 0 is a 90 angle)
    float dotProduct(Vec3 v){
        return (this->x * v.x) + (this->y * v.y) + (this->z * v.z);
    }
    
    // Vector Cross Product
    Vec3* crossProduct(Vec3 v){
        Vec3* result = new Vec3(0,0,0);
        result->x = (this->y * v.z) - (this->z * v.y);
        result->y = (this->z * v.x) - (this->x * v.z);
        result->z = (this->x * v.y) - (this->y * v.x);
    
        return result;
    }
    
    // ---- Static Vector Operations ----
    
    // Static Vector Addition
    static Vec3* add(Vec3 v1, Vec3 v2){
        Vec3* result = new Vec3(0,0,0);
        result->x = v1.x + v2.x;
        result->y = v1.y + v2.y;
        result->z = v1.z + v2.z;
        
        return result;
    }
    
    // Static Vector Subtraction
    static Vec3* sub(Vec3 v1, Vec3 v2){
        Vec3* result = new Vec3(0,0,0);
        result->x = v1.x - v2.x;
        result->y = v1.y - v2.y;
        result->z = v1.z - v2.z;
    
        return result;
    }
    
    // Static Dot Product
    static float dotProduct(Vec3 v1, Vec3 v2){
        return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
    }
    
    // Static Cross Product
    static Vec3* crossProduct(Vec3 v1, Vec3 v2){
        Vec3* result = new Vec3(0,0,0);
        result->x = (v1.y * v2.z) - (v1.z * v2.y);
        result->y = (v1.z * v2.x) - (v1.x * v2.z);
        result->z = (v1.x * v2.y) - (v1.y * v2.x);
    
        return result;
    }
    
}; //ENDOF: Vec3
 

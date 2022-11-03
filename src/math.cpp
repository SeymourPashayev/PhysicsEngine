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
    
}; //ENDOF: Vec3
 

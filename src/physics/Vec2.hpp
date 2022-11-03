//
//  Vec2.hpp
//  engine
//
//  Created by Seymour Pashayev on 11/2/22.
//

#ifndef Vec2_hpp
#define Vec2_hpp

#include "Vec3.hpp"

struct Vec2 {
    float x;
    float y;
    
    Vec2();
    Vec2(float x, float y);
    
    ~Vec2() = default;
    
    void Add(const Vec2& v);
    void Sub(const Vec2& v);
    void Scale(const float n);
    Vec2 Rotate(const float angle) const;
    
    float Magnitude() const;
    float MagnitudeSquared() const;
    
    Vec2& Normalize();
    Vec2 UnitVector() const;
    Vec2 Normal() const;
    
    float Dot(const Vec2& v) const;
    float FloatCross(const Vec2& v) const;
    
    // ---- Operator Overloading ----
    Vec2& operator = (const Vec2& v);           // v1 = v2
    bool operator == (const Vec2& v) const;     // v1 == v2
    bool operator != (const Vec2& v) const;     // v1 != v2
    
    Vec2 operator + (const Vec2& v) const;      // v1 + v2
    Vec2 operator - (const Vec2& v) const;      // v1 - v2
    Vec2 operator * (const float n) const;      // v1 * v2
    Vec2 operator / (const float n) const;      // v1 / v2
    Vec2 operator - ();                         // -v1
    
    Vec2& operator += (const Vec2& v);          // v1 += v2
    Vec2& operator -= (const Vec2& v);          // v1 -= v2
    Vec2& operator *= (const float n);          // v1 *= v2
    Vec2& operator /= (const float n);          // v1 /= v2
};

#endif /* Vec2_hpp */

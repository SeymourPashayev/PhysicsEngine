//
//  Vec3.hpp
//  engine
//
//  Created by Seymour Pashayev on 11/2/22.
//

#ifndef Vec3_hpp
#define Vec3_hpp

struct Vec3 {
    float x;
    float y;
    float z;
    
    Vec3();
    Vec3(float x, float y, float z);
    
    ~Vec3() = default;
    
    void Add(const Vec3& v);
    void Sub(const Vec3& v);
    void Scale(const float n);
    
    float Magnitude() const;
    float MagnitudeSquared() const;
    
    Vec3& Normalize();
    Vec3 UnitVector() const;
    
    float Dot(const Vec3& v) const;
    
    // ---- Operator Overloading ----
    Vec3& operator = (const Vec3& v);           // v1 = v2
    bool operator == (const Vec3& v) const;     // v1 == v2
    bool operator != (const Vec3& v) const;     // v1 != v2
    
    Vec3 operator + (const Vec3& v) const;      // v1 + v2
    Vec3 operator - (const Vec3& v) const;      // v1 - v2
    Vec3 operator * (const float n) const;      // v1 * v2
    Vec3 operator / (const float n) const;      // v1 / v2
    Vec3 operator - ();                         // -v1
    
    Vec3& operator += (const Vec3& v);          // v1 += v2
    Vec3& operator -= (const Vec3& v);          // v1 -= v2
    Vec3& operator *= (const float n);          // v1 *= v2
    Vec3& operator /= (const float n);          // v1 /= v2
    
};

#endif /* Vec3_hpp */

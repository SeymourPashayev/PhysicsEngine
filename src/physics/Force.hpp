#ifndef FORCE_HPP
#define FORCE_HPP

#include "./Vec2.hpp"
#include "./Particle2D.hpp"

struct Force {

    static Vec2 GenerateDragForce(const Particle2D& particle, float dragCoefficient, float area = 1.0);
    static Vec2 GenerateWeightForce(const Particle2D& particle);
    
    static Vec2 GenerateFrictionForce(const Particle2D& particle, float frictionCoefficient);

    static Vec2 GenerateGravitationalForce(const Particle2D& particle1, const Particle2D& particle2, float gravitationalConstant, float minDistance, float maxDistance);

    static Vec2 GenerateSpringForce(const Particle2D& particle, const Vec2& anchor, float springConstant, float restLength);
    static Vec2 GenerateSpringForce(const Particle2D& particle1, const Particle2D& particle2, float springConstant, float restLength);
};

#endif

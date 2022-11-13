#ifndef FORCE_HPP
#define FORCE_HPP

#include "./Vec2.hpp"
#include "./Particle2D.hpp"

struct Force {

    static Vec2 GenerateDragForce(const Particle2D& particle, float dragCoefficient, float area = 1.0);
    static Vec2 GenerateWeightForce(const Particle2D& particle);
    
    static Vec2 GenerateFrictionForce(const Particle2D& particle, float frictionCoefficient);

    static Vec2 GenerateGravitationalForce(const Particle2D& particle1, const Particle2D& particle2, float gravitationalConstant, float minDistance, float maxDistance);
};

#endif

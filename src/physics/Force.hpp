#ifndef FORCE_HPP
#define FORCE_HPP

#include "./Vec2.hpp"
#include "./Particle2D.hpp"

struct Force {

    static Vec2 GenerateDragForce(const Particle2D& particle, float dragCoefficient, float area = 1.0);

};

#endif

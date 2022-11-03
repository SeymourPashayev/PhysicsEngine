#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "Vec2.hpp"

struct Particle {

    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    float mass;
    //float radius;
    //float damping;
    
    Particle(float x, float y, float mass);
    ~Particle();

}; //ENDOF: Particle


#endif

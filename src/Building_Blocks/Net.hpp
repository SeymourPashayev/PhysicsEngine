// Net.hpp
// Created: Tue 15 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// Net class to represent a lattice of point masses connected by springs

#ifndef NET_HPP
#define NET_HPP

// Project Includes
#include "../Physics/Particle2D.hpp"
#include "../Physics/Force.hpp"
#include "../Inputs/Mouse.hpp"

// System Includes
#include <vector>


struct Net{
   
    // Mouse
    Mouse* mouse;

    // Vector of particle pointers
    std::vector<Particle2D*> particles;
    int particleCount = 0;

    // Net Properties
    int width;
    int height;

    // Particle Properties
    float mass = 1.0f;
    float radius = 0.5f;

    // ------------------------

    Net(Mouse* mouse);
    ~Net();
    
    void setSize(int width, int height) { this->width = width; this->height = height; }
    void setMass(float mass) { this->mass = mass; }
    void setRadius(float radius) { this->radius = radius; }

    void Update(float dt, Vec2 pushForce);
    void Draw();



};

#endif

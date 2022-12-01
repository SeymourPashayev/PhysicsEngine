// FluidDynamics2D.hpp
// Created: Wed 23 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------

#ifndef FLUIDDYNAMICS2D_HPP
#define FLUIDDYNAMICS2D_HPP

// Own Classes
#include "../Physics/Vec2.hpp"
#include "../Physics/Particle2D.hpp"
#include "../Graphics/Graphics.hpp"
#include "../Inputs/Mouse.hpp"
#include "../Physics/Force.hpp"
#include "../Physics/Constants.hpp"
#include "System.hpp"

// System Imports
#include <vector>
#include <iostream>


struct FluidDynamics2D: public System {

    std::vector <Vec2> VelocityField;

    // ------------------------

    // create a constructor that initalizes system
    FluidDynamics2D(Mouse* mouse);
    
    // create a destructor that deletes system
    ~FluidDynamics2D();


    void Update(float dt, Vec2 pushForce);
    void Draw();


    // Dynamic System Methods
    Vec2 Advect(Vec2& vel, float dt, Vec2& q);

    // Partcile Creation Methods
    void CreateParticleAtMouse(float dt) ;
};

#endif

// SPH.hpp
// Created: Wed 23 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------

#ifndef SPH_HPP
#define SPH_HPP

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


struct SPH: public System {

    std::vector <Vec2> VelocityField;

    // ------------------------

    // create a constructor that initalizes system
    SPH(Mouse* mouse);
    
    // create a destructor that deletes system
    ~SPH();


    void Update(float dt, Vec2 pushForce);
    void Draw();


    // Dynamic System Methods
    Vec2 Advect(Vec2& vel, float dt, Vec2& q);

    // Partcile Creation Methods
    void CreateParticleAtMouse(float dt) ;

    // Proposed SPH Method (2003 Paper)
    // void InitSPH(void)
    // void Integrate(void)
    // void ComputeDensityPressure(void)
    // void ComputeForces(void)
    // void Update(void)
};

#endif

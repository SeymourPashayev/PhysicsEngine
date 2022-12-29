// SpringForceLattice.hpp
// Created: Tue 15 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------


#ifndef SPRINGFORCELATTICE_HPP
#define SPRINGFORCELATTICE_HPP

// Project Includes
#include "../Physics/Vec2.hpp"
#include "../Physics/Particle2D.hpp"
#include "../Physics/Force.hpp"
#include "../Physics/Spring.hpp"
#include "../Physics/Constants.hpp"
#include "../Inputs/Mouse.hpp"
#include "../System.hpp"

// System Includes
#include <vector>
#include <iostream>

struct SpringForceLattice : public System {

    // Spring Force System Variables
    int width;
    int height;

    // ------------------------------
    
    SpringForceLattice(Mouse* mouse);
    ~SpringForceLattice();

    void Advect(float dt, Vec2 pushForce);

};

#endif

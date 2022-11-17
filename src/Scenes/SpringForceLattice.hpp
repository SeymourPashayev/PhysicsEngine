#ifndef SPRINGFORCELATTICE_HPP
#define SPRINGFORCELATTICE_HPP

// Project Includes
#include "../Physics/Vec2.hpp"
#include "../Physics/Particle2D.hpp"
#include "../Physics/Force.hpp"
#include "../Physics/Constants.hpp"
#include "../Graphics.hpp"
#include "../Inputs/Mouse.hpp"
#include "System.hpp"

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

    void Update(float dt, Vec2 pushForce);

};

#endif

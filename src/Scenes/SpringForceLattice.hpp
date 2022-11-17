#ifndef SPRINGFORCELATTICE_HPP
#define SPRINGFORCELATTICE_HPP

// Project Includes
#include "../Physics/Vec2.hpp"
#include "../Physics/Particle2D.hpp"
#include "../Graphics.hpp"
#include "../Inputs/Mouse.hpp"
#include "../Physics/Force.hpp"
#include "../Physics/Constants.hpp"
#include "System.hpp"

// System Includes
#include <vector>
#include <iostream>

struct SpringForceLattice : public System {



    // ------------------------------
    
    SpringForceLattice(Mouse* mouse);
    ~SpringForceLattice();

};

#endif

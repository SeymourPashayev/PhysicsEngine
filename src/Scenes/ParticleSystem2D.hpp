// ParticleSystem2D.hpp
// Created: Tue 15 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------


#ifndef PARTICLESYSTEM2D_HPP
#define PARTICLESYSTEM2D_HPP

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


struct ParticleSystem2D: public System {

    // create a constructor that initalizes system
    ParticleSystem2D(Mouse* mouse);
    
    // create a destructor that deletes system
    ~ParticleSystem2D();

};

#endif

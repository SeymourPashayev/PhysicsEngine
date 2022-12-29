// WaterDam.hpp
// Created: Thu 22 Dec 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// Water Dam Simulation with SPH

#ifndef WATERDAM_HPP
#define WATERDAM_HPP

// Project Includes
#include "../System.hpp"

// System Includes

struct WaterDam : public System {
   
    // Simulation Variables
    int width;
    int height;

    // ------------------------

    WaterDam(Mouse* mouse);
    ~WaterDam();

};

#endif

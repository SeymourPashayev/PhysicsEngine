// DamBreak.hpp
// Created: Thu 23 Jan 2026
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// Dam Break Simulation with SPH and Boundary Particles

#ifndef DAMBREAK_HPP
#define DAMBREAK_HPP

// Project Includes
#include "../System.hpp"

// System Includes

struct DamBreak : public System {

    // ------------------------

    DamBreak(Mouse* mouse);
    ~DamBreak();

};

#endif

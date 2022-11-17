// ParticleSystem2D.cpp
// Created: Tue 15 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// This is a simple 2D Spring Force Parcile Lattice system scene.

// Project Includes
#include "SpringForceLattice.hpp"

SpringForceLattice::SpringForceLattice(Mouse* mouse) : System { mouse } {

    std::cout << "Spring Force Lattice System Initiated" << std::endl;
    
}

SpringForceLattice::~SpringForceLattice() {
    
    std::cout << "Spring Force Lattice System Destroyed" << std::endl;

}



// ParticleSystem2D.cpp
// Created: Tue 15 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// This is a simple 2D particle system scene.

// Project Includes
#include "ParticleSystem2D.hpp"


ParticleSystem2D::ParticleSystem2D(Mouse* mouse):System { mouse } {

    std::cout << "Particle System Initiated" << std::endl;

}

ParticleSystem2D::~ParticleSystem2D() {

    std::cout << "Particle System Destroyed" << std::endl;

}

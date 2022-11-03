// Particle.cpp
// Created: Thu 03 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// Particle.cpp - A particle struct for the engine

#include <iostream>
#include "Particle.hpp"

Particle::Particle(float x, float y, float mass) {
    this->position = Vec2(x, y);
    this->mass = mass;

    std::cout << "Particle constructor caled." << std::endl;
}

Particle::~Particle() {
    std::cout << "Particle destructor called." << std::endl;
}



// BoundaryParticles.hpp
//
// Created: 2026-01-23
// ------------------------
// Dynamic Boundary Condition (DBC) utilities for SPH fluid containment
// ------------------------

#ifndef BOUNDARYPARTICLES_HPP
#define BOUNDARYPARTICLES_HPP

#include "Particle2D.hpp"
#include <vector>

// Creates 3-layer rectangular container boundary particles
// Uses same spacing and mass as fluid particles for correct SPH interaction
void CreateRectangularContainer(
    std::vector<Particle2D*>& particles,
    float containerWidth,
    float containerHeight,
    float particleSpacing,
    float particleMass
);

#endif

// SPHSolver.hpp
// Created: Thu 23 Jan 2026
// ------------------------
// SPH Solver for density, pressure, and force computations
// Separates SPH logic from particle data structure

#ifndef SPHSOLVER_HPP
#define SPHSOLVER_HPP

#include "Particle2D.hpp"
#include "Octree2D.hpp"
#include "SPHKernel.hpp"
#include "Constants.hpp"
#include <vector>

class SPHSolver {

public:
    // Constructor
    SPHSolver();

    // Initialize the solver with smoothing length
    void Initialize(float smoothingLength);

    // Compute density and pressure for all particles
    void ComputeDensityPressure(std::vector<Particle2D*>& particles, Octree2D* octree);

    // Compute SPH forces (placeholder for Plan 02)
    void ComputeForces(std::vector<Particle2D*>& particles, Octree2D* octree);

private:
    float smoothingLength;  // Kernel radius (h)
    float restDensity;      // Rest density (ρ₀)
    float stiffness;        // Gas constant B for Tait equation
    float gamma;            // Tait exponent (typically 7 for water)
    float viscosity;        // Viscosity coefficient (μ)
};

#endif

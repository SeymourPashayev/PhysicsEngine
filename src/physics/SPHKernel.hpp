// SPHKernel.hpp
// Created: Wed 17 Sep 2025
// ------------------------
// SPH Kernel Functions for Smoothed Particle Hydrodynamics
// Includes Poly6, Spiky, and Viscosity kernels

#ifndef SPHKERNEL_HPP
#define SPHKERNEL_HPP

#include "Vec2.hpp"
#include "Constants.hpp"
#include <cmath>

class SPHKernel {

public:

    // Smoothing length (kernel radius)
    static float h;

    // Precomputed constants for efficiency
    static float poly6Constant;
    static float spikyConstant;
    static float viscosityConstant;

    // Initialize kernel constants
    static void Initialize(float smoothingLength);

    // Poly6 kernel - used for density calculation
    static float Poly6(float r);
    static float Poly6(const Vec2& r);

    // Spiky kernel gradient - used for pressure forces
    static Vec2 SpikyGradient(const Vec2& r);

    // Viscosity kernel laplacian - used for viscosity forces
    static float ViscosityLaplacian(float r);
    static float ViscosityLaplacian(const Vec2& r);

private:
    static constexpr float PI = 3.14159265359f;
};

#endif
// SPHKernel.cpp
// Created: Wed 17 Sep 2025
// ------------------------
// SPH Kernel Functions Implementation

#include "SPHKernel.hpp"

// Static member definitions
float SPHKernel::h = 16.0f;
float SPHKernel::poly6Constant = 0.0f;
float SPHKernel::spikyConstant = 0.0f;
float SPHKernel::viscosityConstant = 0.0f;

void SPHKernel::Initialize(float smoothingLength) {
    h = smoothingLength;

    // Precompute kernel constants for efficiency
    float h2 = h * h;
    float h3 = h2 * h;
    float h6 = h3 * h3;
    float h9 = h6 * h3;

    // Poly6: 315 / (64 * π * h^9)
    poly6Constant = 315.0f / (64.0f * PI * h9);

    // Spiky: -45 / (π * h^6)
    spikyConstant = -45.0f / (PI * h6);

    // Viscosity: 45 / (π * h^6)
    viscosityConstant = 45.0f / (PI * h6);
}

float SPHKernel::Poly6(float r) {
    if (r >= h) return 0.0f;

    float h2_r2 = h * h - r * r;
    return poly6Constant * h2_r2 * h2_r2 * h2_r2;
}

float SPHKernel::Poly6(const Vec2& r) {
    return Poly6(r.Magnitude());
}

Vec2 SPHKernel::SpikyGradient(const Vec2& r) {
    float distance = r.Magnitude();
    if (distance >= h || distance == 0.0f) return Vec2(0.0f, 0.0f);

    float h_r = h - distance;
    float coefficient = spikyConstant * h_r * h_r / distance;

    return Vec2(coefficient * r.x, coefficient * r.y);
}

float SPHKernel::ViscosityLaplacian(float r) {
    if (r >= h) return 0.0f;

    return viscosityConstant * (h - r);
}

float SPHKernel::ViscosityLaplacian(const Vec2& r) {
    return ViscosityLaplacian(r.Magnitude());
}
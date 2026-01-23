// Octree2D.hpp
// Created: Tue 20 Dec 2022
// Updated: Wed 17 Sep 2025 - Complete rewrite for performance and thread safety
// ------------------------
// Seymour Pashayev & Claude
// gitHub:@SeymourPashayev
// ------------------------
// Optimized Octree Data Structure for 2D particle simulation with SPH

#ifndef OCTREE2D_HPP
#define OCTREE2D_HPP

// System Includes
#include <vector>
#include <functional>
#include <memory>
#include <atomic>
#include <mutex>

// Project Includes
#include "Particle2D.hpp"
#include "Vec2.hpp"

class Octree2D {

public:
    // Original interface - keeping compatibility
    Octree2D();
    Octree2D(std::vector<Particle2D*>& particles, const Vec2& min, const Vec2& max, int depth);

    // Destructor
    ~Octree2D();

    // Original methods
    void insert(Particle2D* particle);
    void findNeighbors(std::vector<Particle2D*>& neighbors, Particle2D* particle, double searchRadius);
    void findNeighbors(std::function<void(Particle2D*)> callback, Particle2D* particle, double searchRadius);
    std::vector<Particle2D*> getParticles();
    void Clear();

    // New optimized methods (for future use)
    void rebuild(std::vector<Particle2D*>& particles);
    int getParticleCount() const;
    void printStats() const;

private:

    // Get all the particles in the octree and children
    void getParticles(std::vector<Particle2D*>& particles);

    // Recursive function for building the octree
    void buildOctree(std::vector<Particle2D*>& particles, int depth);

    // Recursive function for finding the neighbors of a particle
    void findNeighbors(std::vector<Particle2D*>& neighbors, Particle2D* particle, double searchRadius, Vec2 min, Vec2 max);

    // Helper methods for optimization
    bool contains(const Vec2& point) const;
    bool intersects(const Vec2& center, float radius) const;
    int getQuadrant(const Vec2& point) const;

    // ------------------------

    // Data members for the octree
    std::vector<Particle2D*> particles;

    Octree2D* children[4];

    Vec2 min;
    Vec2 max;

    int depth;
    int maxDepth = 6;  // Increased from 5 for better performance
    int maxParticlesPerNode = 16;  // New: limit particles per leaf node

    // Thread safety (for future use)
    mutable std::mutex nodeMutex;
};

#endif


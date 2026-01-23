// Octree2D.cpp
// Created: Tue 20 Dec 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// Octree Data Structure for 2-dimensional version of the simulation

// Project Includes
#include "Octree2D.hpp"

// System Includes
#include <iostream>

// Constructor
Octree2D::Octree2D() {
    // Initialize the data members
    particles.clear();

    for (int i = 0; i < 4; i++) {
        children[i] = nullptr;
    }

    min = Vec2();
    max = Vec2();

    depth = 0;
}

// Another Constructor
Octree2D::Octree2D(std::vector<Particle2D*>& particles, const Vec2& min, const Vec2& max, int depth) {
    // Initialize the data members
    this->particles = particles;
   
    for (int i = 0; i < 4; i++) {
        children[i] = nullptr;
    }

    this->min = min;
    this->max = max;
    this->depth = depth;

    // Build the octree recursively
    buildOctree(particles, depth);
}

// Destructor
Octree2D::~Octree2D() {
    // Recursively delete the children
    for (int i = 0; i < 4; i++) {
        if (children[i] != nullptr) {
            delete children[i];
            children[i] = nullptr;
        }
    }
}

void Octree2D::buildOctree(std::vector<Particle2D*>& particles, int depth) {

    // Calculate the center and half-width of the octant
    Vec2 center = (min + max) * 0.5;

    // Divide the particles into four groups based on their position
    std::vector<Particle2D*> lowerLeft;
    std::vector<Particle2D*> lowerRight;
    std::vector<Particle2D*> upperLeft;
    std::vector<Particle2D*> upperRight;
  
    for (int i = 0; i < (int) particles.size(); i++) {
        Particle2D* particle = particles[i];
        if (particle->position.x < center.x) {
            if (particle->position.y < center.y) {
                lowerLeft.push_back(particle);
            } else {
                upperLeft.push_back(particle);
            }
        } else {
            if (particle->position.y < center.y) {
                lowerRight.push_back(particle);
            } else {
                upperRight.push_back(particle);
            }
        }
    }

    // Create child octants if needed
    if (depth < maxDepth) {

        Vec2 minLowerLeft(min.x, min.y);
        Vec2 maxLowerLeft(center.x, center.y);
        children[0] = new Octree2D(lowerLeft, minLowerLeft, maxLowerLeft, depth + 1);

        Vec2 minLowerRight(center.x, min.y);
        Vec2 maxLowerRight(max.x, center.y);
        children[1] = new Octree2D(lowerRight, minLowerRight, maxLowerRight, depth + 1);

        Vec2 minUpperLeft(min.x, center.y);
        Vec2 maxUpperLeft(center.x, max.y);
        children[2] = new Octree2D(upperLeft, minUpperLeft, maxUpperLeft, depth + 1);

        Vec2 minUpperRight(center.x, center.y);
        Vec2 maxUpperRight(max.x, max.y);
        children[3] = new Octree2D(upperRight, minUpperRight, maxUpperRight, depth + 1);
    }
}

void Octree2D::insert(Particle2D* particle) {
    // Check if the particle is within the bounds of the octant
    if (particle->position.x >= min.x && particle->position.x < max.x &&
        particle->position.y >= min.y && particle->position.y < max.y) {

        // If the octant is a leaf node, insert the particle into the particles vector
        if (depth == maxDepth) {
            particles.push_back(particle);
        }
        // Otherwise, insert the particle into the appropriate child octant
        else {
            int quadrant = getQuadrant(particle->position);
            if (quadrant >= 0 && quadrant < 4 && children[quadrant] != nullptr) {
                children[quadrant]->insert(particle);
            }
        }
    }
}

// Update the octree
//void Octree2D::Update() {
//}


// This function will give the particle list of the octant that contains the given particle
void Octree2D::findNeighbors(std::vector<Particle2D*>& neighbors, Particle2D* particle, double searchRadius) {
    
    // Check if the octant is within the search radius
    if (particle->position.x + searchRadius < min.x || particle->position.x - searchRadius >= max.x ||
        particle->position.y + searchRadius < min.y || particle->position.y - searchRadius >= max.y) {
        return;
    }

    // If the octant is a leaf node, check the particles in the particles vector
    if (depth == maxDepth) {
        for (int i = 0; i < (int)particles.size(); i++) {

            Particle2D* p = particles[i];
            
            double distanceSquared = (p->position - particle->position).MagnitudeSquared();
            
            if (distanceSquared < searchRadius * searchRadius) {
                neighbors.push_back(p);
            }

        }
    }

    // Otherwise, search the child octants
    else {
        for (int i = 0; i < 4; i++) {
            children[i]->findNeighbors(neighbors, particle, searchRadius);
        }
    }
}

// This version of findNeighbors is will give a particle to work on
void Octree2D::findNeighbors(std::function<void(Particle2D*)> callback, Particle2D* particle, double searchRadius) {
    // Check if the octant is within the search radius
    if (particle->position.x + searchRadius < min.x || particle->position.x - searchRadius >= max.x ||
        particle->position.y + searchRadius < min.y || particle->position.y - searchRadius >= max.y) {
        return;
    }

    // If the octant is a leaf node, check the particles in the particles vector
    if (depth == maxDepth) {
        for (int i = 0; i < (int) particles.size(); i++) {
            Particle2D* p = particles[i];
            double distanceSquared = (p->position - particle->position).MagnitudeSquared();
            if (distanceSquared < searchRadius * searchRadius) {
                callback(p);
            }
        }
    }
    // Otherwise, search the child octants
    else {
        for (int i = 0; i < 4; i++) {
            children[i]->findNeighbors(callback, particle, searchRadius);
        }
    }
}

void Octree2D::findNeighbors(std::vector<Particle2D*>& neighbors, Particle2D* particle, double searchRadius, Vec2 min, Vec2 max) {
    // Check if the octant is within the search radius and the specified region
    if (particle->position.x + searchRadius < min.x || particle->position.x - searchRadius >= max.x ||
        particle->position.y + searchRadius < min.y || particle->position.y - searchRadius >= max.y ||
        min.x > max.x || min.y > max.y) {
        return;
    }

    // If the octant is a leaf node, check the particles in the particles vector
    if (depth == maxDepth) {
        for (int i = 0; i < (int)particles.size(); i++) {
            Particle2D* p = particles[i];
            double distanceSquared = (p->position - particle->position).MagnitudeSquared();
            if (distanceSquared < searchRadius * searchRadius && p->position.x >= min.x && p->position.x < max.x && p->position.y >= min.y && p->position.y < max.y) {
                neighbors.push_back(p);
            }
        }
    }
    // Otherwise, search the child octants
    else {
        for (int i = 0; i < 4; i++) {
            children[i]->findNeighbors(neighbors, particle, searchRadius, min, max);
        }
    }
}


std::vector<Particle2D*> Octree2D::getParticles() {
    std::vector<Particle2D*> particles;
    getParticles(particles);
    return particles;
}

void Octree2D::getParticles(std::vector<Particle2D*>& particles) {
    particles.insert(particles.end(), this->particles.begin(), this->particles.end());
    for (int i = 0; i < 4; i++) {
        if (children[i] != nullptr) {
            children[i]->getParticles(particles);
        }
    }
}

void Octree2D::Clear() {
    particles.clear();
    for (int i = 0; i < 4; i++) {
        if (children[i] != nullptr) {
            children[i]->Clear();
            delete children[i];
            children[i] = nullptr;
        }
    }
}

// New helper methods for optimization
bool Octree2D::contains(const Vec2& point) const {
    return (point.x >= min.x && point.x < max.x &&
            point.y >= min.y && point.y < max.y);
}

bool Octree2D::intersects(const Vec2& center, float radius) const {
    return !(center.x + radius < min.x || center.x - radius >= max.x ||
             center.y + radius < min.y || center.y - radius >= max.y);
}

int Octree2D::getQuadrant(const Vec2& point) const {
    Vec2 center = (min + max) * 0.5f;

    if (point.x < center.x) {
        if (point.y < center.y) {
            return 0; // Lower left
        } else {
            return 2; // Upper left
        }
    } else {
        if (point.y < center.y) {
            return 1; // Lower right
        } else {
            return 3; // Upper right
        }
    }
}

void Octree2D::rebuild(std::vector<Particle2D*>& newParticles) {
    Clear();
    buildOctree(newParticles, depth);
}

int Octree2D::getParticleCount() const {
    int count = particles.size();
    for (int i = 0; i < 4; i++) {
        if (children[i] != nullptr) {
            count += children[i]->getParticleCount();
        }
    }
    return count;
}

void Octree2D::printStats() const {
    std::cout << "Octree Stats - Depth: " << depth
              << ", Particles: " << particles.size()
              << ", Total in subtree: " << getParticleCount() << std::endl;
}



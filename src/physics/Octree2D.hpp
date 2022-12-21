// Octree2D.hpp
// Created: Tue 20 Dec 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// Octree Data Structure for 2-dimensional version of the simulation

// System Includes
#include <vector>

// Project Includes
#include "Particle2D.hpp"

class Octree2D {

public:
    // Constructor
    Octree2D();
    Octree2D(std::vector<Particle2D*>& particles, const Vec2& min, const Vec2& max, int depth);

    // Destructor
    ~Octree2D();

    // Insert a particle into the octree
    void insert(Particle2D* particle);

    // Find the neighbors of a particle within a specified search radius
    void findNeighbors(std::vector<Particle2D*>& neighbors, Particle2D* particle, double searchRadius);
    void findNeighbors(std::function<void(Particle2D*)> callback, Particle2D* particle, double searchRadius);    
    
    // get all the particles
    std::vector<Particle2D*> getParticles();

private:
    
    // Get all the particles in the octree and children
    void getParticles(std::vector<Particle2D*>& particles);

    // Recursive function for building the octree
    void buildOctree(std::vector<Particle2D*>& particles, int depth);

    // Recursive function for finding the neighbors of a particle
    void findNeighbors(std::vector<Particle2D*>& neighbors, Particle2D* particle, double searchRadius, Vec2 min, Vec2 max); 

    void Clear();

    // ------------------------

    // Data members for the octree
    std::vector<Particle2D*> particles;

    Octree2D* children[4];
  
    Vec2 min;
    Vec2 max;
  
    int depth;
    int maxDepth = 5;
};


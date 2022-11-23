// Spring.hpp
// Created: Wed 23 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// Spring class to be used in an array in System class

#ifndef SPRING_HPP
#define SPRING_HPP

// Project Includes
#include "Particle2D.hpp"
#include "../Graphics/Graphics.hpp"

// System Includes


struct Spring {
   
    // Spring Variables
    Particle2D *p1;
    Particle2D *p2;

    float restLength;
    float springConstant;

    // ------------------------
    
    Spring(Particle2D *p1, Particle2D *p2, float restLength, float springConstant);
    ~Spring();

    void Update();
    void Draw();

};

#endif //ENDOF SPRING_HPP

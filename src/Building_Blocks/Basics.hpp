#ifndef BASICS_HPP
#define BASICS_HPP

// Project Includes
#include "../Physics/Vec2.hpp"

// System Includes

struct liquid {

   Vec2 position;
   float height;
   float width;

   float densityCoefficient;

   void Update(float dt);
   void Render();
};

#endif

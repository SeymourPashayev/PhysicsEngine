// Spring.cpp
// Created: Wed 23 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------


#include "Spring.hpp"

Spring::Spring(Particle2D *p1, Particle2D *p2, float restLength, float springConstant) {
    this->p1 = p1;
    this->p2 = p2;
    this->restLength = restLength;
    this->springConstant = springConstant;
}

Spring::~Spring() {
    std::cout << "Spring Destroyed" << std::endl;
}

void Spring::Update() {
    // Calculate the vector between the two particles
    Vec2 delta = p2->position - p1->position;
    float distance = delta.Magnitude();
    float difference = distance - restLength;
    float springForce = difference * springConstant;

    // Calculate the force to apply to each particle
    Vec2 force = delta.Normalize() * springForce;
    p1->AddForce(force);
    p2->AddForce(-force);
}

void Spring::Draw() {
    Graphics::DrawLine(p1->position.x, p1->position.y, p2->position.x, p2->position.y, 0x00FF00);
}


#include "Mouse.hpp"
#include <iostream>

// Constructor
Mouse::Mouse() {
    std::cout << "Mouse created" << std::endl;
}

// Destructor
Mouse::~Mouse() {
    std::cout << "Mouse destroyed" << std::endl;
}
    
void Mouse::IncreaseCursorSize(float increment) {

    if (cursorSize + increment > maxCursorSize || cursorSize + increment < minCursorSize) {
        return;
    }

    cursorSize += increment;
}

void Mouse::UpdatePosition(Vec2 pos) {

    prevPosition.x = position.x;
    prevPosition.y = position.y;
    position.x = pos.x;
    position.y = pos.y;

}

void Mouse::printCurrentLocation() {
    std::cout << "Mouse is at: " << position.x << ", " << position.y << std::endl;
}


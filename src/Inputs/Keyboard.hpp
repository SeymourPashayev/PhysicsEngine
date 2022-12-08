// Keyboard.hpp
// Created: Thu 08 Dec 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// Keyboard Functionality for SDL2 Game Engine

#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

// Project Includes

// System Includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>

struct Keyboard {
    
    // ----
    
    Keyboard();
    ~Keyboard();
    
    void Update();
    
    bool GetKey(SDL_Keysym  KeySym) const; 
    bool SetKey(SDL_Keysym KeySym, bool state);

    bool ToggleKey(SDL_Keysym KeySym);
   
};


#endif //ENDOF: Keyboard.hpp

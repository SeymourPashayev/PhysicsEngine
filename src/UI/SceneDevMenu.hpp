// SceneDevMenu.hpp
// Created: Thu 17 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------

#ifndef SCENEDEVMENU_HPP
#define SCENEDEVMENU_HPP

// Project Includes
#include "../Graphics/Text.hpp"

// System Includes
#include <vector>
#include <iostream>

struct SceneDevMenu {
    
    // Renderer
    SDL_Renderer* renderer;

    // Scene Data to Show
    std::vector<std::string> sceneData;

    // Textbox for Data
    Text* TextBox;

    // -------------------
    
    SceneDevMenu(SDL_Renderer* renderer);
    ~SceneDevMenu();

    void Update();
    void Draw();

};

#endif

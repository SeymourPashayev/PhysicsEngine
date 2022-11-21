// SceneDevMenu.cpp
// Created: Thu 17 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// Scene Developer Menu. This is to be used for debugging and development purposes only. It shows the current scene variabels and other important data

// Project Includes
#include "SceneDevMenu.hpp"

SceneDevMenu::SceneDevMenu(SDL_Renderer* renderer) {
    
    // Set up the renderer
    this->renderer = renderer;

    TextBox = new Text(renderer);

    //this->TextBox->setText("SceneDevMenu");
    
    std::cout << "SceneDevMenu Created" << std::endl;

}

SceneDevMenu::~SceneDevMenu() {

    std::cout << "SceneDevMenu Destroyed" << std::endl;

}

void SceneDevMenu::Update() {
    
    // Move the data to be shown from sceneData to TextBox
    std::string formattedData = "";
    for (auto data : this->sceneData) {
        formattedData += data;
    }

    this->TextBox->setText(formattedData);

}

void SceneDevMenu::Draw() {

    this->TextBox->Draw();

}

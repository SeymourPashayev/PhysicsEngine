// Text.cpp
// Created: Thu 17 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------
// Text.cpp is to represent an SDL Text Field that will be used in the simulation

// Project Includes
#include "Text.hpp"


Text::Text(SDL_Renderer* renderer) {
    
    // Set the position of the text box
    position.x = 0;
    position.y = 0;
   
    // TODO: Try to move the font loading and error checking into Graphics::OpenWindow() [function] and Graphics::GlobalFont [variable]
    // Load the font
    this->font = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 72);
    
    // Check if the font got loaded
    if (font==NULL){
        printf("Failed to load font: %s \n", SDL_GetError());
        SDL_Quit();
    }
    
    // Set the renderer
    this->renderer = renderer;

    std::cout << "Text Box Created" << std::endl;

}


Text::~Text() {

     SDL_DestroyTexture(texture);

     std::cout << "Text Box Destroyed" << std::endl;

}


void Text::setText(std::string text){

    if (this->text==text){
        return;
    }

    this->text = text;

    SDL_DestroyTexture(texture);
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);

    if (surface==NULL) {
        printf("Failed to render text: %s", SDL_GetError());            
    }

    position.w = surface->w;
    position.h = surface->h;

    texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);
}


void Text::Draw () {
    SDL_RenderCopy(renderer, texture, NULL, &position);
}


Text& Text::operator=(const std::string& text) {
    setText(text);
    return *this;
}

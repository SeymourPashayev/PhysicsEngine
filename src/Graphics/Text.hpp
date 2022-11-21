// Text.hpp
// Created: Thu 17 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------

#ifndef TEXT_HPP
#define TEXT_HPP

// Project Includes
#include "Graphics.hpp"

// System Includes
#include <string>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

struct Text {

    SDL_Texture* texture = NULL;
    SDL_Rect position;
    TTF_Font* font;
    SDL_Renderer* renderer;

    SDL_Color color = *new SDL_Color{255, 255, 255, 255};
    std::string text;

    // -------------------

    Text(SDL_Renderer* renderer);
    ~Text();

    void setText(const std::string text);
    std::string getText() const { return text; }

    void Draw();
    
    // --------------------------------

    Text& operator=(const std::string& text);

};


#endif //ENDOF: TEXT_HPP

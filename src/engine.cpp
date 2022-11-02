#include <SDL2/SDL.h>
#include <iostream>

SDL_Window* window = 0;
SDL_Renderer* renderer = 0;
bool running = false;

bool init(const char* title, int xpos, int ypos, int height, int width, int flags) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) >= 0) {
        // If succsessfull, create window
        // Temprorary Hardcoded Solution
        window = SDL_CreateWindow("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);

        if (window == NULL) {
            // In the case that the window could not be made...
            std::cout << "Window is NULL" << std::endl;
            return 1;
        }

        // if window creation successful, create a renderer
        if (window != 0) {
            std::cout << "GOT TO RENDERER" << std::endl;
            renderer = SDL_CreateRenderer(window, -1, 0);
        }
    } else {
        // Couldn't initialize SDL
        return false;
    }

    return true;
}

void render(){
    // set to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // clear the window to black
    SDL_RenderClear(renderer);

    // show the window
    SDL_RenderPresent(renderer);
}

int main (int argc, char* argv[]){
    if (init("TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0)) {
        running = true;
        std::cout << "Error: " << SDL_GetError() << std::endl;
    } else {
        // error
        return 1;
    }

    while (running) {
        render();
    }

    // Cleanup
    SDL_Quit();

    return 0;
}

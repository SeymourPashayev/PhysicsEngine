#include <SDL2/SDL.h>
#include <iostream>

// SDL Variables
SDL_Window* win = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* bitmapTex = NULL;
SDL_Surface* bitmapSurface = NULL;
int xpos = 100, ypos = 100, width = 320, height = 240;

// Flow Control
SDL_bool loopShouldStop = SDL_FALSE;

SDL_bool init(const char* title, int xpos, int ypos, int height, int width) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) >= 0) {
        // If succsessfull, create window
        win = SDL_CreateWindow(title, xpos, ypos, width, height, 0);

        // if window creation successful, create a renderer
        if (win != 0) {
            renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
        }

        // Set-up the image
        bitmapSurface = SDL_LoadBMP("../img/test.jpg");
        bitmapTex = SDL_CreateTextureFromSurface(renderer, bitmapSurface);
        SDL_FreeSurface(bitmapSurface);
        
    } else {
        // Couldn't initialize SDL
        return SDL_TRUE;
    }

    return SDL_TRUE;
}

void render(){
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bitmapTex, NULL, NULL);
    SDL_RenderPresent(renderer);
}

int main (int argc, char* argv[]){
    if (init("TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480)) {
        std::cout << "Error: " << SDL_GetError() << std::endl;
    } else {

        // error
        return 1;
    }

    while (true) {
        render();
    }

    // Cleanup
    SDL_DestroyTexture(bitmapTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}

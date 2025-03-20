#include "Utils.h"
#include "Constants.h"

int initSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logError(std::cout, "SDL_Init");
        return 1;
    }
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        logError(std::cout, "IMG_Init");
        SDL_Quit();
        return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        logError(std::cout, "Mix_OpenAudio");
        SDL_Quit();
        return 1;
    }
    return 0;
}

SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
    if (!texture) {
        logError(std::cout, "Load " + path);
    }
    return texture;
}

void logError(std::ostream& os, const std::string& msg) {
    os << msg << " error: " << SDL_GetError() << std::endl;
}

void clearRenderer(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);  // Màu nền xanh nhạt
    SDL_RenderClear(renderer);
}

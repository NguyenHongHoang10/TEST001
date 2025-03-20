#ifndef UTILS_H
#define UTILS_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <string>

int initSDL();
SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer);
void logError(std::ostream& os, const std::string& msg);
void clearRenderer(SDL_Renderer* renderer);

#endif

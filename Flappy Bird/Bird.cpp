#include "Bird.h"
#include "Constants.h"
#include "Utils.h"

Bird::Bird(SDL_Renderer* renderer) : renderer(renderer), x(SCREEN_WIDTH / 4), y(SCREEN_HEIGHT / 2), velocity(0), angle(0) {
    texture = loadTexture("bird.png", renderer);
    flapSound = Mix_LoadWAV("flap_sound.mp3");
    if (!texture || !flapSound) {
        logError(std::cout, "Failed to load bird resources");
        throw std::runtime_error("Bird resource loading failed");
    }
}

Bird::~Bird() {
    SDL_DestroyTexture(texture);
    Mix_FreeChunk(flapSound);
}

void Bird::flap() {
    velocity = FLAP_FORCE;
    Mix_PlayChannel(-1, flapSound, 0);
}

void Bird::update() {
    velocity += GRAVITY;
    y += static_cast<int>(velocity);
    if (y < 0) y = 0;
    if (y + BIRD_HEIGHT > SCREEN_HEIGHT - GROUND_HEIGHT) y = SCREEN_HEIGHT - GROUND_HEIGHT - BIRD_HEIGHT + 15;
    angle = -velocity * 5;  // Xoay chim dựa trên vận tốc
}

void Bird::render() {
    SDL_Rect rect = {x, y, BIRD_WIDTH, BIRD_HEIGHT};
    SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
}

SDL_Rect Bird::getRect() {
    return {x, y, 48, 48};
}

void Bird::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

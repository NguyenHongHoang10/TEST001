#ifndef BIRD_H
#define BIRD_H

#include <SDL.h>
#include <SDL_mixer.h>

class Bird {
private:
    SDL_Renderer* renderer;
    int x, y;
    float velocity;
    SDL_Texture* texture;
    Mix_Chunk* flapSound;
    double angle;

public:
    Bird(SDL_Renderer* renderer);
    ~Bird();
    void flap();
    void update();
    void render();
    SDL_Rect getRect();
    void setPosition(int newX, int newY);
};

#endif

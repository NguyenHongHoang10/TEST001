#ifndef PIPE_H
#define PIPE_H

#include <SDL.h>
#include <SDL_mixer.h>

class Pipe {
private:
    SDL_Renderer* renderer;
    int x;
    int topHeight;
    int bottomY;
    SDL_Texture* topTexture;
    SDL_Texture* bottomTexture;
    bool passed;
    Mix_Chunk* swooshSound;
    Mix_Chunk* pointSound;

public:
    Pipe(SDL_Renderer* renderer, int startX);
    ~Pipe();
    void update();
    void render(int shakeOffsetX, int shakeOffsetY);
    void reset(int startX);
    SDL_Rect getTopRect(int shakeOffsetX, int shakeOffsetY);
    SDL_Rect getBottomRect(int shakeOffsetX, int shakeOffsetY);
    bool hasPassed();
    void markPassed();
};

#endif

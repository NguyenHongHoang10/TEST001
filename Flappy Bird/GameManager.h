#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include "Bird.h"
#include "Pipe.h"

enum GameState { MENU, PLAYING, GAME_OVER };

class GameManager {
private:
    SDL_Renderer* renderer;
    Bird* bird;
    std::vector<Pipe*> pipes;
    SDL_Texture* background;
    SDL_Texture* ground;
    SDL_Texture* startButton;
    SDL_Texture* gameOverTexture;
    SDL_Texture* restartButton;
    Mix_Chunk* hitSound;
    Mix_Chunk* dieSound;
    TTF_Font* font; // Font để hiển thị điểm số
    SDL_Texture* scoreTexture; // Texture để hiển thị điểm số
    int score;
    bool gameOver;
    GameState state;
    float menuTimer;
    float groundX;
    int shakeTimer;
    int shakeOffsetX;
    int shakeOffsetY;

    void handleEvents(SDL_Event& e);
    void update();
    void render();
    bool checkCollision();
    void startGame();
    void updateScoreTexture(); // Hàm để cập nhật texture điểm số

public:
    GameManager(SDL_Renderer* renderer);
    ~GameManager();
    void run(bool& restart);
};

#endif

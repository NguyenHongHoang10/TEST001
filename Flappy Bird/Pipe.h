#ifndef PIPE_H
#define PIPE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include "Constants.h" // Thêm include để sử dụng các hằng số
#include "Utils.h"
class Pipe {
private:
    SDL_Renderer* renderer;
    SDL_Texture* endTexture;  // Texture cho phần vòm (pipe_end.png)
    SDL_Texture* bodyTexture; // Texture cho phần thân (pipe_body.png)
    Mix_Chunk* swooshSound;   // Âm thanh khi chim bay qua
    Mix_Chunk* pointSound;    // Âm thanh khi ghi điểm
    int x;                    // Vị trí ngang của cột
    int topHeight;            // Chiều cao cột trên
    int bottomY;              // Vị trí y của đầu cột dưới
    bool passed;              // Cột đã được chim vượt qua chưa
    int endHeight;            // Chiều cao phần vòm
    int bodyHeight;           // Chiều cao một đoạn thân

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

#include "Pipe.h"
#include "Constants.h"
#include "Utils.h"
#include <cstdlib>

Pipe::Pipe(SDL_Renderer* renderer, int startX) : renderer(renderer), x(startX), passed(false) {
    int gapSize = rand() % (GAP_MAX - GAP_MIN + 1) + GAP_MIN;
    int gapCenter = rand() % (MAX_GAP_CENTER - MIN_GAP_CENTER + 1) + MIN_GAP_CENTER;
    topHeight = gapCenter - gapSize / 2;
    bottomY = gapCenter + gapSize / 2;

    topTexture = loadTexture("top_pipe.png", renderer);
    bottomTexture = loadTexture("bot_pipe.png", renderer);
    swooshSound = Mix_LoadWAV("swoosh_sound.mp3");
    pointSound = Mix_LoadWAV("point_sound.mp3");
    if (!topTexture || !bottomTexture || !swooshSound || !pointSound) {
        logError(std::cout, "Failed to load pipe resources");
        throw std::runtime_error("Pipe resource loading failed");
    }
}

Pipe::~Pipe() {
    SDL_DestroyTexture(topTexture);
    SDL_DestroyTexture(bottomTexture);
    Mix_FreeChunk(swooshSound);
    Mix_FreeChunk(pointSound);
}

void Pipe::update() {
    x -= SCROLL_SPEED;
}

void Pipe::render(int shakeOffsetX, int shakeOffsetY) {
    SDL_Rect topRect = {x + shakeOffsetX, 0 + shakeOffsetY, PIPE_WIDTH, topHeight};
    SDL_Rect bottomRect = {x + shakeOffsetX, bottomY + shakeOffsetY, PIPE_WIDTH, SCREEN_HEIGHT - GROUND_HEIGHT - bottomY};
    SDL_RenderCopy(renderer, topTexture, NULL, &topRect);
    SDL_RenderCopy(renderer, bottomTexture, NULL, &bottomRect);
}

void Pipe::reset(int startX) {
    x = startX;
    passed = false;
    // Tạo khoảng trống ngẫu nhiên
    int gapSize = rand() % (GAP_MAX - GAP_MIN + 1) + GAP_MIN;
    //int gapSize = 0;
    int gapCenter = rand() % (MAX_GAP_CENTER - MIN_GAP_CENTER + 1) + MIN_GAP_CENTER;
    // Đảm bảo khoảng trống không quá gần mép trên hoặc mép dưới
    if (gapCenter - gapSize / 2 < 30) {
        gapCenter = 30 + gapSize / 2;
    }
    if (gapCenter + gapSize / 2 > SCREEN_HEIGHT - GROUND_HEIGHT - 30) {
        gapCenter = SCREEN_HEIGHT - GROUND_HEIGHT - 30 - gapSize / 2;
    }
    // Đảm bảo khoảng trống đủ lớn (thêm kiểm tra bổ sung)
    if (gapSize < GAP_MIN) {
        gapSize = GAP_MIN;
    }
    if (gapSize > GAP_MAX) {
        gapSize = GAP_MAX;
    }
    std::cout << "Pipe reset: x=" << x << ", gapSize=" << gapSize << ", gapCenter=" << gapCenter
              << ", topHeight=" << (gapCenter - gapSize / 2)
              << ", bottomY=" << (gapCenter + gapSize / 2) << std::endl;
    topHeight = gapCenter - gapSize / 2;
    bottomY = gapCenter + gapSize / 2;
}

SDL_Rect Pipe::getTopRect(int shakeOffsetX, int shakeOffsetY) {
    return {x + shakeOffsetX + COLLISION_OFFSET, 0 + shakeOffsetY, PIPE_WIDTH - 2 * COLLISION_OFFSET, topHeight};
}

SDL_Rect Pipe::getBottomRect(int shakeOffsetX, int shakeOffsetY) {
    return {x + shakeOffsetX + COLLISION_OFFSET, bottomY + shakeOffsetY, PIPE_WIDTH - 2 * COLLISION_OFFSET, SCREEN_HEIGHT - GROUND_HEIGHT - bottomY};
}

bool Pipe::hasPassed() {
    return passed;
}

void Pipe::markPassed() {
    if (!passed) {
        passed = true;
        Mix_PlayChannel(-1, swooshSound, 0);
        Mix_PlayChannel(-1, pointSound, 0);
    }
}

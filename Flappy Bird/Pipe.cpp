#include "Pipe.h"
#include <cstdlib>


Pipe::Pipe(SDL_Renderer* renderer, int startX) : renderer(renderer), x(startX), passed(false) {
    // Tạo khoảng trống ngẫu nhiên giữa cột trên và cột dưới
    int gapSize = rand() % (GAP_MAX - GAP_MIN + 1) + GAP_MIN; // Sử dụng GAP_MIN và GAP_MAX
    int gapCenter = rand() % (MAX_GAP_CENTER - MIN_GAP_CENTER + 1) + MIN_GAP_CENTER; // Sử dụng MIN_GAP_CENTER và MAX_GAP_CENTER
    topHeight = gapCenter - gapSize / 2;
    bottomY = gapCenter + gapSize / 2;

    // Tải texture
    endTexture = loadTexture("pipe_end.png", renderer);
    bodyTexture = loadTexture("pipe_body.png", renderer);
    swooshSound = Mix_LoadWAV("swoosh_sound.mp3");
    pointSound = Mix_LoadWAV("point_sound.mp3");

    if (!endTexture || !bodyTexture || !swooshSound || !pointSound) {
        std::cerr << "Failed to load pipe resources" << std::endl;
        throw std::runtime_error("Pipe resource loading failed");
    }

    // Lấy chiều cao của texture
    SDL_QueryTexture(endTexture, NULL, NULL, NULL, &endHeight);
    SDL_QueryTexture(bodyTexture, NULL, NULL, NULL, &bodyHeight);

}

Pipe::~Pipe() {
    SDL_DestroyTexture(endTexture);
    SDL_DestroyTexture(bodyTexture);
    Mix_FreeChunk(swooshSound);
    Mix_FreeChunk(pointSound);
}

void Pipe::update() {
    x -= SCROLL_SPEED; // Sử dụng SCROLL_SPEED thay vì 2
}

void Pipe::render(int shakeOffsetX, int shakeOffsetY) {
    // Vẽ cột trên
    if (topHeight > 0) {
        // Vẽ phần vòm ở cuối cột trên (treo ngược)
        int capHeight = std::min(endHeight, topHeight);
        SDL_Rect topEndDst = {x + shakeOffsetX, topHeight - capHeight + shakeOffsetY, PIPE_WIDTH, capHeight}; // Sử dụng PIPE_WIDTH thay vì 52
        SDL_RenderCopy(renderer, endTexture, NULL, &topEndDst);

        // Lặp lại phần thân từ mép trên đến ngay trên phần vòm
        int remainingHeight = topHeight - capHeight;
        if (remainingHeight > 0) {
            for (int y = 0; y < remainingHeight; y += bodyHeight) {
                int segmentHeight = std::min(bodyHeight, remainingHeight - y);
                SDL_Rect topBodyDst = {x + shakeOffsetX, y + shakeOffsetY, PIPE_WIDTH, segmentHeight};
                SDL_RenderCopy(renderer, bodyTexture, NULL, &topBodyDst);
            }
        }
    }

    // Vẽ cột dưới
    int bottomHeight = SCREEN_HEIGHT - GROUND_HEIGHT - bottomY; // Sử dụng SCREEN_HEIGHT và GROUND_HEIGHT
    if (bottomHeight > 0) {
        // Vẽ phần vòm ở đầu cột dưới
        int capHeight = std::min(endHeight, bottomHeight);
        SDL_Rect bottomEndDst = {x + shakeOffsetX, bottomY + shakeOffsetY, PIPE_WIDTH, capHeight};
        SDL_RenderCopy(renderer, endTexture, NULL, &bottomEndDst);

        // Lặp lại phần thân từ ngay dưới phần vòm đến mặt đất
        int remainingHeight = bottomHeight - capHeight;
        if (remainingHeight > 0) {
            int targetY = SCREEN_HEIGHT - GROUND_HEIGHT; // Mặt đất
            int startY = bottomY + capHeight;
            int y = startY;
            int iteration = 0;
            while (y < targetY) {
                int segmentHeight = std::min(bodyHeight, targetY - y);
                if (segmentHeight <= 0) break; // Thoát nếu không còn đoạn nào để vẽ
                SDL_Rect bottomBodyDst = {x + shakeOffsetX, y + shakeOffsetY, PIPE_WIDTH, segmentHeight};
                SDL_RenderCopy(renderer, bodyTexture, NULL, &bottomBodyDst);
                y += segmentHeight;
                iteration++;
            }
            // Debug để kiểm tra
//            std::cout << "Bottom pipe: bottomY=" << bottomY << ", bottomHeight=" << bottomHeight
//                      << ", capHeight=" << capHeight << ", remainingHeight=" << remainingHeight
//                      << ", iterations=" << iteration << std::endl;
        }
    }
}

void Pipe::reset(int startX) {
    x = startX;
    passed = false;
    // Tạo khoảng trống ngẫu nhiên
    int gapSize = rand() % (GAP_MAX - GAP_MIN + 1) + GAP_MIN;
    int gapCenter = rand() % (MAX_GAP_CENTER - MIN_GAP_CENTER + 1) + MIN_GAP_CENTER;
    // Đảm bảo cột không quá ngắn
    if (gapCenter - gapSize / 2 < endHeight) {
        gapCenter = endHeight + gapSize / 2;
    }
    if (gapCenter + gapSize / 2 > SCREEN_HEIGHT - GROUND_HEIGHT - endHeight) {
        gapCenter = SCREEN_HEIGHT - GROUND_HEIGHT - endHeight - gapSize / 2;
    }
    topHeight = gapCenter - gapSize / 2;
    bottomY = gapCenter + gapSize / 2;
}

SDL_Rect Pipe::getTopRect(int shakeOffsetX, int shakeOffsetY) {
    //return {x + shakeOffsetX + COLLISION_OFFSET, shakeOffsetY, PIPE_WIDTH - 2 * COLLISION_OFFSET, topHeight};
    int collisionWidth = std::max(0, PIPE_WIDTH - 2 * COLLISION_OFFSET);
    SDL_Rect topRect = {x + shakeOffsetX + COLLISION_OFFSET, shakeOffsetY, collisionWidth, topHeight - 18};



    return topRect;
}

SDL_Rect Pipe::getBottomRect(int shakeOffsetX, int shakeOffsetY) {
    //return {x + shakeOffsetX + COLLISION_OFFSET, bottomY + shakeOffsetY, PIPE_WIDTH - 2 * COLLISION_OFFSET, SCREEN_HEIGHT - GROUND_HEIGHT - bottomY};
    int collisionWidth = std::max(0, PIPE_WIDTH - 2 * COLLISION_OFFSET);
    SDL_Rect bottomRect = {x + shakeOffsetX + COLLISION_OFFSET, bottomY + shakeOffsetY, collisionWidth, SCREEN_HEIGHT - GROUND_HEIGHT - bottomY};


    return bottomRect;
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

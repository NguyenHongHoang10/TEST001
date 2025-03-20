#include "GameManager.h"
#include "Constants.h"
#include "Utils.h"
#include <cmath>

GameManager::GameManager(SDL_Renderer* renderer) : renderer(renderer), score(0), gameOver(false), state(MENU), menuTimer(0), groundX(0), shakeTimer(0), shakeOffsetX(0), shakeOffsetY(0) {
    bird = new Bird(renderer);
    for (int i = 0; i < 100; ++i) {
        pipes.push_back(new Pipe(renderer, SCREEN_WIDTH + i * PIPE_SPACING));
    }
    background = loadTexture("background.png", renderer);
    ground = loadTexture("ground.png", renderer);
    startButton = loadTexture("start_button.png", renderer);
    gameOverTexture = loadTexture("game_over.png", renderer);
    restartButton = loadTexture("restart_button.png", renderer);
    hitSound = Mix_LoadWAV("bird_hit_sound.mp3");
    dieSound = Mix_LoadWAV("die_sound.mp3");
    if (!background || !ground || !startButton || !gameOverTexture || !restartButton || !hitSound || !dieSound) {
        logError(std::cout, "Failed to load resources in GameManager");
        throw std::runtime_error("Resource loading failed");
    }

    if (TTF_Init() == -1) {
        logError(std::cout, "TTF_Init failed: " + std::string(TTF_GetError()));
        throw std::runtime_error("SDL_ttf initialization failed");
    }

    // Khởi tạo font
    font = TTF_OpenFont(FONT_PATH.c_str(), FONT_SIZE);
    if (!font) {
        logError(std::cout, "TTF_OpenFont failed: " + std::string(TTF_GetError()));
        throw std::runtime_error("Font loading failed");
    }



    // Khởi tạo texture điểm số
    updateScoreTexture();

        // Đọc điểm số cao nhất từ file
    std::ifstream inFile("highscore.txt");
    if (inFile.is_open()) {
        inFile >> highScore;
        inFile.close();
        updateHighScoreTexture();
    }

    highScoreTexture = nullptr;
    updateHighScoreTexture(); // Khởi tạo texture điểm số cao nhất
}

GameManager::~GameManager() {
    delete bird;
    for (auto& pipe : pipes) delete pipe;
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(ground);
    SDL_DestroyTexture(startButton);
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(restartButton);
    SDL_DestroyTexture(highScoreTexture);
    TTF_Quit();
    Mix_FreeChunk(hitSound);
    Mix_FreeChunk(dieSound);
    Mix_CloseAudio();
}

void GameManager::updateScoreTexture() {
    // Chuyển điểm số thành chuỗi
    std::stringstream ss;
    if (state == GAME_OVER) {
        ss << "Score: " << score; // Hiển thị tổng điểm khi game over
    } else {
        ss << "Score: " << score; // Hiển thị điểm số trong lúc chơi
    }
    std::string scoreText = ss.str();

    // Render văn bản thành surface
    SDL_Color textColor = {255, 165, 0, 255}; // Màu cam
    SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    if (!surface) {
        logError(std::cout, "TTF_RenderText_Solid failed: " + std::string(TTF_GetError()));
        return;
    }

    // Tạo texture từ surface
    if (scoreTexture) {
        SDL_DestroyTexture(scoreTexture);
        scoreTexture = nullptr;

    scoreTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!scoreTexture) {
        logError(std::cout, "SDL_CreateTextureFromSurface failed: " + std::string(SDL_GetError()));
    // Tạo texture mặc định nếu thất bại
        surface = TTF_RenderText_Solid(font, "Error", textColor);
        if (surface) {
            scoreTexture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }
    }
    }

}
void GameManager::updateHighScoreTexture() {
    // Chuyển điểm số cao nhất thành chuỗi
    std::stringstream ss;
    ss << "High Score: " << highScore;
    std::string highScoreText = ss.str();

    // Render văn bản thành surface
    SDL_Color textColor = {0, 0, 255, 255}; // Xanh lam
    SDL_Surface* surface = TTF_RenderText_Solid(font, highScoreText.c_str(), textColor);
    if (!surface) {
        logError(std::cout, "TTF_RenderText_Solid failed: " + std::string(TTF_GetError()));
        return;
    }

    // Tạo texture từ surface
    if (highScoreTexture) {
        SDL_DestroyTexture(highScoreTexture);
        highScoreTexture = nullptr;
    }
    highScoreTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!highScoreTexture) {
        logError(std::cout, "SDL_CreateTextureFromSurface failed: " + std::string(SDL_GetError()));
    }
}

void GameManager::run(bool& restart) {
    restart = false;
    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) quit = true;
            handleEvents(e);
        }

        if (state != GAME_OVER) {
            update();
            if (state == PLAYING && checkCollision()) {
                gameOver = true;
                state = GAME_OVER;
                Mix_PlayChannel(-1, hitSound, 0);
                Mix_PlayChannel(-1, dieSound, 0);
                std::cout << "Game Over! SCORE: " << score << std::endl;
                shakeTimer = 10;
                updateScoreTexture(); // Cập nhật texture điểm số khi game over
                if (score > highScore) {
                    highScore = score;
                    updateHighScoreTexture(); // Cập nhật texture điểm số cao nhất
                    // Lưu điểm số cao nhất vào file
                    std::ofstream outFile("highscore.txt");
                    if (outFile.is_open()) {
                        outFile << highScore;
                        outFile.close();
                    }
                }
            }
        }

        render();
        SDL_Delay(1);
    }
}

void GameManager::handleEvents(SDL_Event& e) {
    if (state == MENU) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mx, my;
            SDL_GetMouseState(&mx, &my);
            int buttonX = SCREEN_WIDTH / 2 - 50;
            int buttonY = SCREEN_HEIGHT / 2 + 50;
            if (mx >= buttonX && mx <= buttonX + 100 && my >= buttonY && my <= buttonY + 100) {
                startGame();
            }
        } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
            startGame();
        }
    } else if (state == PLAYING) {
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
            bird->flap();
        }
    } else if (state == GAME_OVER) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mx, my;
            SDL_GetMouseState(&mx, &my);
            int buttonX = SCREEN_WIDTH / 2 - 50;
            int buttonY = SCREEN_HEIGHT / 2 + 50;
            if (mx >= buttonX && mx <= buttonX + 100 && my >= buttonY && my <= buttonY + 100) {
                startGame();
            }
        } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
            startGame();
        }
    }
}

void GameManager::update() {
    if (state == MENU) {
        menuTimer += 0.05;
        bird->setPosition(SCREEN_WIDTH / 4, static_cast<int>(SCREEN_HEIGHT / 2 + sin(menuTimer) * 30));
    } else if (state == PLAYING) {
        bird->update();
        // Tìm cột có vị trí x lớn nhất
        int maxPipeX = pipes[0]->getTopRect(0, 0).x;
        for (const auto& pipe : pipes) {
            int pipeX = pipe->getTopRect(0, 0).x;
            if (pipeX > maxPipeX) {
                maxPipeX = pipeX;
            }
        }
        // Cập nhật và đặt lại cột
        for (auto& pipe : pipes) {
            pipe->update();
            if (pipe->getTopRect(0, 0).x + PIPE_WIDTH < bird->getRect().x && !pipe->hasPassed()) {
                pipe->markPassed();
                score++;
                std::cout << "Diem: " << score << std::endl;
                updateScoreTexture();
            }
            if (pipe->getTopRect(0, 0).x + PIPE_WIDTH < 0) {
                // Đặt lại cột ở vị trí maxPipeX + PIPE_SPACING
                pipe->reset(maxPipeX + PIPE_SPACING);
                std::cout << "Pipe reset at x=" << pipe->getTopRect(0, 0).x << std::endl;
                // Cập nhật maxPipeX
                maxPipeX = pipe->getTopRect(0, 0).x;
            }
        }
        updateScoreTexture();
        groundX -= SCROLL_SPEED;
        if (groundX < -SCREEN_WIDTH) groundX += SCREEN_WIDTH;
    }

    if (shakeTimer > 0) {
        shakeOffsetX = (rand() % 10) - 5;
        shakeOffsetY = (rand() % 10) - 5;
        shakeTimer--;
    } else {
        shakeOffsetX = 0;
        shakeOffsetY = 0;
    }
}

void GameManager::render() {
    clearRenderer(renderer);
    SDL_Rect backgroundRect = {shakeOffsetX, shakeOffsetY, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, background, NULL, &backgroundRect);

    for (auto& pipe : pipes) pipe->render(shakeOffsetX, shakeOffsetY);
    bird->render();
    SDL_Rect groundRect = {static_cast<int>(groundX) + shakeOffsetX, SCREEN_HEIGHT - GROUND_HEIGHT + shakeOffsetY, SCREEN_WIDTH, GROUND_HEIGHT};
    SDL_Rect groundRect2 = {static_cast<int>(groundX + SCREEN_WIDTH) + shakeOffsetX, SCREEN_HEIGHT - GROUND_HEIGHT + shakeOffsetY, SCREEN_WIDTH, GROUND_HEIGHT};
    SDL_RenderCopy(renderer, ground, NULL, &groundRect);
    SDL_RenderCopy(renderer, ground, NULL, &groundRect2);
    if (state == MENU) {
        SDL_Rect buttonRect = {SCREEN_WIDTH / 2 - 50 + shakeOffsetX, SCREEN_HEIGHT / 2 + 50 + shakeOffsetY, 100, 100};
        SDL_RenderCopy(renderer, startButton, NULL, &buttonRect);
    } else if (state == GAME_OVER) {
        SDL_Rect gameOverRect = {SCREEN_WIDTH / 2 - 150 + shakeOffsetX, SCREEN_HEIGHT / 2 - 150 + shakeOffsetY, 300, 100};
        SDL_Rect restartRect = {SCREEN_WIDTH / 2 - 50 + shakeOffsetX, SCREEN_HEIGHT / 2 + 50 + shakeOffsetY, 100, 100};
        SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect);
        SDL_RenderCopy(renderer, restartButton, NULL, &restartRect);
    }

    if (highScoreTexture && state == MENU) {
    int w, h;
    SDL_QueryTexture(highScoreTexture, NULL, NULL, &w, &h);
    SDL_Rect highScoreDst = {SCREEN_WIDTH / 2 - w / 2 + shakeOffsetX, SCREEN_HEIGHT / 2 - 50 + shakeOffsetY, w, h};
    SDL_RenderCopy(renderer, highScoreTexture, NULL, &highScoreDst);
    }

    // Hiển thị điểm số
    if (scoreTexture && state != MENU) {
        int w, h;
        SDL_QueryTexture(scoreTexture, NULL, NULL, &w, &h);
        SDL_Rect scoreDst;
        if (state == GAME_OVER) {
            // Hiển thị điểm số ở giữa màn hình, dưới game over texture
            scoreDst = {SCREEN_WIDTH / 2 - w / 2 + shakeOffsetX, SCREEN_HEIGHT / 2 - 40 + shakeOffsetY, w, h};
        } else if (state == PLAYING) {
            // Hiển thị điểm số ở góc trên giữa
            scoreDst = {SCREEN_WIDTH / 2 - w / 2 + shakeOffsetX, SCORE_POS_Y + shakeOffsetY, w, h};
        }
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreDst);
    }

    // Hiển thị điểm số cao nhất khi game over
    if (highScoreTexture && state == GAME_OVER) {
        int w, h;
        if (SDL_QueryTexture(highScoreTexture, NULL, NULL, &w, &h) != 0) {
            std::cout << "SDL_QueryTexture failed for highScoreTexture: " << SDL_GetError() << std::endl;
            return;
        }
        SDL_Rect highScoreDst = {SCREEN_WIDTH / 2 - w / 2 + shakeOffsetX, SCREEN_HEIGHT / 2 + 10 + shakeOffsetY, w, h};
        SDL_RenderCopy(renderer, highScoreTexture, NULL, &highScoreDst);
    }
    SDL_RenderPresent(renderer);
}

bool GameManager::checkCollision() {
    SDL_Rect birdRect = bird->getRect();
    for (auto& pipe : pipes) {
        SDL_Rect topRect = pipe->getTopRect(shakeOffsetX, shakeOffsetY);
        SDL_Rect bottomRect = pipe->getBottomRect(shakeOffsetX, shakeOffsetY);
        if (SDL_HasIntersection(&birdRect, &topRect)) {
            std::cout << "Collision with top pipe: birdRect(x=" << birdRect.x << ", y=" << birdRect.y
                      << ", w=" << birdRect.w << ", h=" << birdRect.h
                      << "), topRect(x=" << topRect.x << ", y=" << topRect.y
                      << ", w=" << topRect.w << ", h=" << topRect.h << ")" << std::endl;
            return true;
        }
        if (SDL_HasIntersection(&birdRect, &bottomRect)) {
            std::cout << "Collision with bottom pipe: birdRect(x=" << birdRect.x << ", y=" << birdRect.y
                      << ", w=" << birdRect.w << ", h=" << birdRect.h
                      << "), bottomRect(x=" << bottomRect.x << ", y=" << bottomRect.y
                      << ", w=" << bottomRect.w << ", h=" << bottomRect.h << ")" << std::endl;
            return true;
        }
    }

    if (birdRect.y + BIRD_HEIGHT >= SCREEN_HEIGHT - GROUND_HEIGHT ) {
        std::cout << "Collision with ground: birdRect(y=" << birdRect.y << ")" << std::endl;
        return true;
    }
    return false;
}

void GameManager::startGame() {
    bird->setPosition(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2);
    bird->flap();
    for (size_t i = 0; i < pipes.size(); ++i) {
        pipes[i]->reset(SCREEN_WIDTH + i * PIPE_SPACING);
    }
    score = 0;
    updateScoreTexture(); // Cập nhật texture điểm số
    gameOver = false;
    state = PLAYING;
}

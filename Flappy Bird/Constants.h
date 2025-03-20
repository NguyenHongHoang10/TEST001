#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

const std::string WINDOW_TITLE = "Flappy Bird";
const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 600;
const int BIRD_WIDTH = 64;
const int BIRD_HEIGHT = 64;
const int PIPE_WIDTH = 52;
const int PIPE_SPACING = 250;
const int GAP_MIN = 135; //180
const int GAP_MAX = 135; //220
const int COLLISION_OFFSET = 0; // Thêm lại để điều chỉnh vùng va chạm
const float GRAVITY = 0.4f;
const float FLAP_FORCE = -7.0f;
const int SCROLL_SPEED = 2;
const int MIN_GAP_CENTER = 120;
const int MAX_GAP_CENTER = 350;
const int GROUND_HEIGHT = 100;

// Hằng số cho hiển thị điểm số
const std::string FONT_PATH = "font.ttf"; // Đường dẫn đến font
const int FONT_SIZE = 24; // Kích thước font
const int SCORE_POS_X = 10; // Vị trí x của điểm số trong lúc chơi
const int SCORE_POS_Y = 10; // Vị trí y của điểm số trong lúc chơi
const int GAME_OVER_SCORE_POS_X = SCREEN_WIDTH / 2 - 50; // Vị trí x của điểm số khi game over
const int GAME_OVER_SCORE_POS_Y = SCREEN_HEIGHT / 2; // Vị trí y của điểm số khi game over

#endif

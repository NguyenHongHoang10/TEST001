#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

const std::string WINDOW_TITLE = "Flappy Bird";
const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 600;
const int BIRD_WIDTH = 68;
const int BIRD_HEIGHT = 48;
const int PIPE_WIDTH = 52;
const int PIPE_SPACING = 200;
const int GAP_MIN = 135; //180
const int GAP_MAX = 135; //220
const int COLLISION_OFFSET = 50; // Thêm lại để điều chỉnh vùng va chạm
const float GRAVITY = 0.4f;
const float FLAP_FORCE = -7.0f;
const int SCROLL_SPEED = 12;
const int MIN_GAP_CENTER = 30;
const int MAX_GAP_CENTER = 400;
const int GROUND_HEIGHT = 100;

#endif

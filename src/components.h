#pragma once
 
#include <SDL3/SDL.h>

struct Position { 
    float x, y;
};

struct Size {
    float w, h;
};

struct Velocity {
    float vx, vy;
};

struct PlayerInput{
    int player_num;
    bool up, down;
};

struct Renderable{
    float x, y;
    SDL_Color color;
};

struct BallPhysics{
    float default_speed;
    float speed;
    float crossing_time;
    float max_speed_mult;
    float speed_increase;
};

struct PaddlePhysics{
    float speed;
    float moving_time;
};

struct Score{
    int left;
    int right;
};

struct GameFlags{
    bool running;
    bool start_game;
    bool scored;
};
#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <cmath>


struct Paddle {
    float x, y, w, h, speed;
};

struct Ball {
    float x, y, w, h, x_velo, y_velo, default_speed, speed;
};

struct GameState {
    Ball ball;
    Paddle left_paddle;
    Paddle right_paddle;
    int left_score;
    int right_score;
    bool running;
    bool start_game;
    bool score;
};

void RenderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, float x, float y);

bool CheckCollision(Paddle& a, Ball& b);

void HandleInput(GameState& state, SDL_Event& event, float delta_seconds, int window_height);

void Update(GameState& state, float delta_seconds, int window_width, int window_height, float speed_increase);

void Render(const GameState& state, SDL_Renderer* renderer, TTF_Font* font, int window_width, int window_height);
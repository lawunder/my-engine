#pragma once
 
#include "world.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
 
// ─── Systems are free functions that operate on the World ───────────────────
// Each system queries only the components it needs. It has no knowledge of
// which specific entities exist — it just iterates component stores.
 
// Replaces: HandleInput() — split into two distinct responsibilities
 
// 1. Process the SDL event queue (quit, resize, key-down events)
void EventSystem(World& world, SDL_Event& event, int& window_width, int& window_height);
 
// 2. Read held keys each frame and update PlayerInput components
void InputSystem(World& world);
 
// Replaces: the paddle movement + clamping logic inside HandleInput()
void PaddleMovementSystem(World& world, float dt, int window_height);
 
// Replaces: ball movement in Update()
void BallMovementSystem(World& world, float dt);
 
// Replaces: collision detection in Update() — now generic (any two AABBs)
void CollisionSystem(World& world);
 
// Replaces: scoring and ball reset logic in Update()
void ScoringSystem(World& world, int window_width, int window_height);
 
// Replaces: Render() — draws all entities with Position + Renderable
void RenderSystem(const World& world, SDL_Renderer* renderer, TTF_Font* font, int window_width, int window_height);
 
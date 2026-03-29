#pragma once
 
#include "components.h"
#include <unordered_map>

using Entity = uint32_t;

struct World{
    std::unordered_map<Entity, Position> positions; 
    std::unordered_map<Entity, Size> sizes; 
    std::unordered_map<Entity, Velocity> velocities; 
    std::unordered_map<Entity, PlayerInput> player_inputs; 
    std::unordered_map<Entity, Renderable> renderables; 
    std::unordered_map<Entity, BallPhysics> ball_physics; 
    std::unordered_map<Entity, PaddlePhysics> paddle_physics; 

    // Singleton components (one per world)
    Score score; 
    GameFlags game_flags; 

    Entity ball_entity = 0;
    Entity left_paddle = 1;
    Entity right_paddle = 2;

    Entity next_ID = 0;
    Entity CreateEntity(){
        return next_ID++;
    };

};
#pragma once
 
#include "components.h"
#include <unordered_map>

struct World{
    std::unordered_map<uint32_t, Position> positions; 
    std::unordered_map<uint32_t, Size> sizes; 
    std::unordered_map<uint32_t, Velocity> velocities; 
    std::unordered_map<uint32_t, PlayerInput> player_inputs; 
    std::unordered_map<uint32_t, Renderable> renderables; 
    std::unordered_map<uint32_t, BallPhysics> ball_physics; 
    std::unordered_map<uint32_t, PaddlePhysics> paddle_physics; 

    // Singleton components (one per world)
    Score score; 
    GameFlags game_flags; 

    uint32_t CreateEntity(){
        return nextId ++;
    };

};
#include "systems.h"
#include <cmath>
#include <string>
#include <cstdlib>


void EventSystem(World& world, SDL_Event& event, int& window_width, int& window_height);
    while (SDL_PollEvent(&event)){
        if (event.type == SDL_EVENT_QUIT){
            state.running = false;
        }
        if (event.type == SDL_EVENT_KEY_DOWN){
            if (event.key.key == SDLK_ESCAPE){
                state.running = false;
            }
        }

        if (event.type == SDL_EVENT_WINDOW_RESIZED){
            int new_x = event.window.data1;
            int new_y = event.window.data2;

            window_width = new_x
            window_height = new_y

            for (Entity e : {world.left_paddle, world.right_paddle}){

                auto& pad_phys = world.paddle_physics[e];
                auto& pos = world.positions[e]
                auto& r = world.renderables[e];
                
                // update paddle height, center, and speed
                r.h = window_height / pad_phys.sizes[e].h;
                pos.y = (window_height / 2.0f) - (render.h / 2.0f);
                pad_phys.speed = window_height / pad_phys.moving_time;
                
            }
            
            // right paddle adjust
            auto& right_paddle = world.renderables[world.rightPaddle];
            world.positions[world.rightPaddle].x = window_width - right_paddle.w;

            // ball speed adjust
            auto& ball_phys = world.ball_phyics[world.ball_entity];
            ball_phys.default_speed = window_width / ball_phys.crossing_time;

            // only reposition ball if game hasn't started
            if (!world.game_flags.start_game){
                world.positions[world.ball_entity] = {window_width/2.0f, window_height/2.0f};
                ball_phys.speed = ball_phys.default_speed;
            }
        }        
}


void RenderSystem(const World& world, SDL_Renderer* renderer, TTF_Font* font, int window_width, int window_height){
    // background
    SDL_SetRenderDrawColor(renderer, 1, 1, 1, 255); 
    SDL_RenderClear(renderer);

    // for each entity, renderable pair
    for (const auto& [e, r] : world.renderables){
        if (!world.positions.count(e)){ 
            continue;
        }
        const auto& pos = world.positions.at(e);

        SDL_SetRenderDrawColor(renderer, r.color.r, r.color.g, r.color.b, 255);
        SDL_FRect rect = {pos.x , pos.y, r.w, r.h};
    }

    std::string left_score = std::to_string(world.score.left);
    std::string right_score = std::to_string(world.score.right);
    RenderText(renderer, font, left_score.c_str(), 10, 10);
    RenderText(renderer, font, right_score.c_str(), window_width - 20, 10);

    if (world.score.left == 5)
        RenderText(renderer, font, "LEFT WINS!", window_width / 2.0f, window_height / 2.0f);
    if (world.score.right == 5)
        RenderText(renderer, font, "RIGHT WINS!", window_width / 2.0f, window_height / 2.0f);

    SDL_RenderPresent(renderer);

}



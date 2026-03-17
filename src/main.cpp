#include "game.h"
#include "world.h"
#include <SDL3/SDL_main.h>






void InitWorld(World& world, float w, float h){
        uint32_t ball = world.CreateEntity();
        world.positions[ball] = {w/2.0f, h/2.0f};        world.sizes[ball] = {25.0f, 25.0f};
        world.velocities[ball] = {1, 0.5f};
        world.renderables[ball] = {w, h, {255,255,255,255}};
        world.ball_physics[ball] = {200.0f, 0.0f, 2.0f, 4.0f, 1.15f};
}


int main(int argc, char* argv[])
{
    std::cout << "Starting...\n"; 

    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)){
        std::cerr << "SDL failed to initialize: " << SDL_GetError() << "\n";
        return -1;
    }

    // Create a window
    int window_width = 1280;
    int window_height = 720;
    SDL_Window* window = SDL_CreateWindow("My Engine", window_width, window_height, SDL_WINDOW_RESIZABLE);
    if (!window){
        std::cerr << "Failed to create window: " << SDL_GetError() << "\n";
        SDL_Quit();
        return -1;
    }
    
    // Create Renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer){
        std::cerr << "Failed to create renderer: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Create Font Loader
    if (!TTF_Init()){
        std::cerr << "SDL_ttf failed to initialize: " << SDL_GetError() << "\n";
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Load Font
    TTF_Font* font = TTF_OpenFont("./assets/fonts/Roboto-VariableFont_wdth,wght.ttf", 30.0f);
    if (!font){
        std::cerr << "Font failed to load: " << SDL_GetError() << "\n";
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    World world;
    InitWorld(world, window_width, window_height);


    // // Vars
    srand(SDL_GetTicks()); // set random seed
    SDL_Event event; // queue of events (inputs)
    Uint64 current_time = SDL_GetTicks();
    Uint64 previous_time = SDL_GetTicks();

    // // PADDLES init
    // float paddle_width = 30;
    // float paddle_size = 3.0f;
    // float paddle_height = window_height / paddle_size;
    // float moving_time = 1.0f; // seconds to cross screen
    // float paddle_speed = window_height / moving_time;
    

    // // BALL init
    // float ball_size = 25.0f;
    // float ball_x_velo;
    // if (rand() % 2 == 0){
    //     ball_x_velo = 1;
    // }
    // else{
    //     ball_x_velo = -1;
    // }
    // float ball_y_velo = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
    // float crossing_time = 2.0f; // seconds to cross screen
    // float ball_speed = window_width / crossing_time;
    // float speed_increase = 1.10;
    // float max_speed_mult = 4;

    // // GAMESTATE init
    // GameState state;
    // state.ball = {window_width/2.0f, window_height/2.0f, ball_size, ball_size, ball_x_velo, ball_y_velo, ball_speed, ball_speed, crossing_time, max_speed_mult, speed_increase};
    // state.left_paddle = {0, ((window_height/2.0f) - (paddle_height/2.0f)), paddle_width, paddle_height, paddle_speed, moving_time, paddle_size};
    // state.right_paddle = {window_width - paddle_width, ((window_height/2.0f) - (paddle_height/2.0f)), paddle_width, paddle_height, paddle_speed, moving_time, paddle_size};
    // state.left_score = 0;
    // state.right_score = 0;
    // state.running = true;
    // state.start_game = false;
    // state.score = false;


    // LOOP
    while (world.game_flags.running){
        // Delta Time
        current_time = SDL_GetTicks();
        float delta = current_time - previous_time;  //delta is difference between last update in ms
        float delta_seconds = delta / 1000.0f; // divide delta  to get amount in 
        previous_time = current_time;

        // SDL_GetWindowSize(window, &window_width, &window_height);
        // HandleInput(state, event, delta_seconds, window_width, window_height);
        // Update(state, delta_seconds, window_width, window_height);
        // Render(state, renderer, font, window_width, window_height);
    
        // SDL_Delay(32); // delay by 32ms, roughly 30fps

        SDL_Delay(5000);
        world.game_flags.running = false;
    }

    // Cleanup all opened objects
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
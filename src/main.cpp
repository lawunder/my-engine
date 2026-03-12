#include "game.h"
#include <SDL3/SDL_main.h>


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
    SDL_Window* window = SDL_CreateWindow("My Engine", window_width, window_height, 0);
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

    // Vars
    srand(SDL_GetTicks()); // set random seed
    SDL_Event event; // queue of events (inputs)
    Uint64 current_time = SDL_GetTicks();
    Uint64 previous_time = SDL_GetTicks();

    // PADDLES init
    float paddle_width = 25;
    float paddle_height = 300;
    float paddle_speed = 400;

    // BALL init
    float ball_size = 20;
    float ball_x_velo;
    if (rand() % 2 == 0){
        ball_x_velo = 1;
    }
    else{
        ball_x_velo = -1;
    }
    float ball_y_velo = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
    float ball_speed = 600;
    float speed_increase = 1.10;

    // GAMESTATE init
    GameState state;
    state.ball = {window_width/2.0f, window_height/2.0f, ball_size, ball_size, ball_x_velo, ball_y_velo, ball_speed, ball_speed};
    state.left_paddle = {0, ((window_height/2.0f) - (paddle_height/2.0f)), paddle_width, paddle_height, paddle_speed};
    state.right_paddle = {window_width - paddle_width, ((window_height/2.0f) - (paddle_height/2.0f)), paddle_width, paddle_height, paddle_speed};
    state.left_score = 0;
    state.right_score = 0;
    state.running = true;
    state.start_game = false;
    state.score = false;


    // LOOP
    while (state.running){
        // Delta Time
        current_time = SDL_GetTicks();
        float delta = current_time - previous_time;  //delta is difference between last update in ms
        float delta_seconds = delta / 1000.0f; // divide delta  to get amount in 
        previous_time = current_time;

        SDL_GetWindowSize(window, &window_width, &window_height);
        HandleInput(state, event, delta_seconds, window_height);
        Update(state, delta_seconds, window_width, window_height, speed_increase);
        Render(state, renderer, font, window_width, window_height);
    
        // SDL_Delay(32); // delay by 32ms, roughly 30fps
    }

    // Cleanup all opened objects
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
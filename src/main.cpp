#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
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


void RenderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, float x, float y){
    //Takes in text and renders it to screen at location
    SDL_Color font_color = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, 0, font_color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    SDL_FRect rect_text;
    rect_text.x = x;
    rect_text.y = y;
    rect_text.w = 0; // handled by get texture size
    rect_text.h = 0; // handled by get texture size
    SDL_GetTextureSize(texture, &rect_text.w, &rect_text.h);
    SDL_RenderTexture(renderer, texture, NULL, &rect_text);
    SDL_DestroyTexture(texture);
}

bool CheckCollision(Paddle& a, Ball& b){
    // AABB collison check
    return (a.x < b.x + b.w && 
        a.x + a.w > b.x && 
        a.y < b.y + b.h && 
        a.y + a.h > b.y);
}

void HandleInput(GameState& state, SDL_Event& event, float delta_seconds, int window_height){ 
    // Process queue of events until no more
    while (SDL_PollEvent(&event)){
        if (event.type == SDL_EVENT_QUIT){
            state.running = false;
        }
        if (event.type == SDL_EVENT_KEY_DOWN){
            if (event.key.key == SDLK_ESCAPE){
                state.running = false;
            }
        }
    }

    // Start Game
    const bool* keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_SPACE]){
        state.start_game = true;
    }

    // left paddle movement
    if (keystate[SDL_SCANCODE_W]){
        state.left_paddle.y -= (delta_seconds * state.left_paddle.speed);
        if (state.left_paddle.y <= 0)
            state.left_paddle.y = 0;
    }
    if (keystate[SDL_SCANCODE_S]){
        state.left_paddle.y += (delta_seconds * state.left_paddle.speed);
        if (state.left_paddle.y >= (window_height - state.left_paddle.h))
            state.left_paddle.y = (window_height - state.left_paddle.h);
    }

    // right paddle movement
    if (keystate[SDL_SCANCODE_UP]){
        state.right_paddle.y -= (delta_seconds * state.right_paddle.speed);
        if (state.right_paddle.y <= 0)
            state.right_paddle.y = 0;
    }
    if (keystate[SDL_SCANCODE_DOWN]){
        state.right_paddle.y += (delta_seconds * state.right_paddle.speed);
        if (state.right_paddle.y >= (window_height - state.right_paddle.h))
            state.right_paddle.y = (window_height - state.right_paddle.h);
    }
}

void Update(GameState& state, float delta_seconds, int window_width, int window_height, float speed_increase){
    if (state.start_game){
        //ball movement and collisions
        float xmove = (delta_seconds * state.ball.speed) * state.ball.x_velo;
        float ymove = (delta_seconds * state.ball.speed) * state.ball.y_velo;
        float move_norm = std::pow(state.ball.x_velo, 2) + std::pow(state.ball.y_velo, 2); // Normalize velo vector
        state.ball.x += (xmove / move_norm);
        state.ball.y += (ymove / move_norm);

        // flip y_velo on floor/ceiling
        if (state.ball.y <= 0){
            state.ball.y_velo *= -1;
        }
        if (state.ball.y >= (window_height - state.ball.h)){
            state.ball.y_velo *= -1;
        }

        // Ball left
        if (CheckCollision(state.left_paddle, state.ball)){
            state.ball.x_velo *= -1;
            state.ball.x = state.left_paddle.x + state.left_paddle.w;
            state.ball.speed *= speed_increase;
        }
        if (state.ball.x <= 0){
            state.right_score += 1;
            state.score = true;
            state.ball.speed = state.ball.default_speed;
        }

        // Ball right
        if (CheckCollision(state.right_paddle, state.ball)){
            state.ball.x_velo *= -1;
            state.ball.x = state.right_paddle.x - state.ball.w;
            state.ball.speed *= speed_increase;
        }
        if (state.ball.x >= window_width){
            state.left_score += 1;
            state.score = true;                
            state.ball.speed = state.ball.default_speed;
        }
        
        // Cap speed
        if (state.ball.speed > 1500.0f){
            state.ball.speed = 1500.0f;
        }
    }

    // Reset Ball on score
    if (state.score){
        state.ball.x = window_width/2.0f;
        state.ball.y = window_height/2.0f;
        if (SDL_GetTicks() % 2 == 0){
            state.ball.x_velo = 1;
        }
        else{
            state.ball.x_velo = -1;
        }
        state.ball.y_velo = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
        state.score = false;
        state.start_game = false;
    }
}


void Render(const GameState& state, SDL_Renderer* renderer, TTF_Font* font, int window_width, int window_height){
        // Create Paddle Objects
        SDL_FRect left_paddle_rect;
        left_paddle_rect.x = state.left_paddle.x;
        left_paddle_rect.y = state.left_paddle.y;
        left_paddle_rect.w = state.left_paddle.w;
        left_paddle_rect.h = state.left_paddle.h;

        SDL_FRect right_paddle_rect;
        right_paddle_rect.x = state.right_paddle.x;
        right_paddle_rect.y = state.right_paddle.y;
        right_paddle_rect.w = state.right_paddle.w;
        right_paddle_rect.h = state.right_paddle.h;

        SDL_FRect ball_rect;
        ball_rect.x = state.ball.x;
        ball_rect.y = state.ball.y;
        ball_rect.w = state.ball.w;
        ball_rect.h = state.ball.h;

        // DRAW
        SDL_SetRenderDrawColor(renderer, 1, 1, 1, 255); // background
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 123, 123, 123, 255); // paddle color
        SDL_RenderFillRect(renderer, &left_paddle_rect);
        SDL_RenderFillRect(renderer, &right_paddle_rect);
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // ball color
        SDL_RenderFillRect(renderer, &ball_rect);

        // Render all text for scoring and win conditions
        std::string left_score_str = std::to_string(state.left_score);
        const char* left_score_text = left_score_str.c_str();
        RenderText(renderer, font, left_score_text, 10, 10);
        std::string right_score_str = std::to_string(state.right_score);
        const char* right_score_text = right_score_str.c_str();
        RenderText(renderer, font, right_score_text, window_width - 20, 10);
        if (state.left_score == 5){
            const char* left_win_text = "LEFT WINS!";
            RenderText(renderer, font, left_win_text, window_width / 2.0f, window_height / 2.0f);
        }
        if (state.right_score == 5){
            const char* right_win_text = "RIGHT WINS!";
            RenderText(renderer, font, right_win_text, window_width / 2.0f, window_height / 2.0f);
        }
        // RENDER
        SDL_RenderPresent(renderer);
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
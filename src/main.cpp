#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>



struct Paddle {
    float x, y, w, h, speed;
};

struct Ball {
    float x, y, w, h, x_velo, y_velo, speed;
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


int main(int argc, char* argv[])
{
    std::cout << "Starting...\n"; 

    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)){
        std::cerr << "SDL failed to initialize: " << SDL_GetError() << "\n";
        return -1;
    }

    // Create a window
    float window_width = 1280;
    float window_height = 720;
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

    if (!TTF_Init()){
        std::cerr << "SDL_ttf failed to initialize: " << SDL_GetError() << "\n";
        SDL_Quit();
        return -1;
    }

    TTF_Font* font = TTF_OpenFont("./assets/fonts/Roboto-VariableFont_wdth,wght.ttf", 30.0f);
    if (!font){
        std::cerr << "Font failed to load: " << SDL_GetError() << "\n";
        SDL_Quit();
        return -1;
    }

    bool running = true;
    SDL_Event event; // queue of events (inputs)
    int current_time = SDL_GetTicks();
    int previous_time = SDL_GetTicks();


    float paddle_width = 25;
    float paddle_height = 150;
    float paddle_speed = 400;
    Paddle left_paddle = {0, ((window_height/2.0f) - (paddle_height/2.0f)), paddle_width, paddle_height, paddle_speed};
    Paddle right_paddle = {window_width - paddle_width, ((window_height/2.0f) - (paddle_height/2.0f)), paddle_width, paddle_height, paddle_speed};

    float ball_size = 20;
    srand(SDL_GetTicks()); // set random seed
    float ball_x_velo;
    if (rand() % 2 == 0){
        ball_x_velo = 1;
    }
    else{
        ball_x_velo = -1;
    }
    float ball_y_velo = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
    float ball_speed = 600;
    Ball ball = {window_width/2.0f, window_height/2.0f, ball_size, ball_size, ball_x_velo, ball_y_velo, ball_speed};

    bool score = false;
    int left_score = 0;
    int right_score = 0;
    bool start_game = false;

    while (running){
        // Process queue of events until no more
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_EVENT_QUIT){
                running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN){
                if (event.key.key == SDLK_ESCAPE){
                    running = false;
                }
            }
        }



        // Handle Delta Time
        current_time = SDL_GetTicks();
        float delta = current_time - previous_time;  //delta is difference between last update in ms
        float delta_seconds = delta / 1000.0f; // divide delta  to get amount in 
        previous_time = current_time;

        // Initialize window sizes and input
        int window_width, window_height;
        SDL_GetWindowSize(window, &window_width, &window_height);
        const bool* keystate = SDL_GetKeyboardState(NULL);

        if (keystate[SDL_SCANCODE_SPACE]){
            start_game = true;
        }

        // left paddle movement
        if (keystate[SDL_SCANCODE_W]){
            left_paddle.y -= (delta_seconds * left_paddle.speed);
            if (left_paddle.y <= 0)
              left_paddle.y = 0;
        }
        
        if (keystate[SDL_SCANCODE_S]){
            left_paddle.y += (delta_seconds * left_paddle.speed);
            if (left_paddle.y >= (window_height - left_paddle.h))
              left_paddle.y = (window_height - left_paddle.h);
        }

        // right paddle movement
        if (keystate[SDL_SCANCODE_UP]){
            right_paddle.y -= (delta_seconds * right_paddle.speed);
            if (right_paddle.y <= 0)
              right_paddle.y = 0;
        }
        if (keystate[SDL_SCANCODE_DOWN]){
            right_paddle.y += (delta_seconds * right_paddle.speed);
            if (right_paddle.y >= (window_height - right_paddle.h))
              right_paddle.y = (window_height - right_paddle.h);
        }

        if (start_game){
            //ball movement and collisions
            ball.x += (delta_seconds * ball.speed) * ball.x_velo;
            ball.y += (delta_seconds * ball.speed) * ball.y_velo;
            if (ball.y <= 0){
                ball.y_velo *= -1;
            }
            if (ball.y >= (window_height - ball.h)){
                ball.y_velo *= -1;
            }

            // Ball enters left scoring zone
            if (ball.x <= left_paddle.w){
                if (ball.y >= left_paddle.y && ball.y <= left_paddle.h + left_paddle.y){
                    ball.x_velo *= -1;
                    ball.speed *= 1.15;
                }
                if (ball.x <= 0){
                    right_score += 1;
                    score = true;
                    ball.speed = ball_speed;
                }
            }

            // Ball enters right scoring zone
            if (ball.x >= window_width - right_paddle.w - ball.w){
                if (ball.y >= right_paddle.y && ball.y <= right_paddle.h + right_paddle.y){
                    ball.x_velo *= -1;
                    ball.speed *= 1.25;
                }
                if (ball.x >= window_width){
                    left_score += 1;
                    score = true;
                    ball.speed = ball_speed;
                }
            }
        }

        if (score){
            ball.x = window_width/2.0f;
            ball.y = window_height/2.0f;
            if (SDL_GetTicks() % 2 == 0){
                ball.x_velo = 1;
            }
            else{
                ball.x_velo = -1;
            }
            ball.y_velo = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
            score = false;
            start_game = false;
        }
        

        // Create Paddle Objects
        SDL_FRect left_paddle_rect;
        left_paddle_rect.x = left_paddle.x;
        left_paddle_rect.y = left_paddle.y;
        left_paddle_rect.w = left_paddle.w;
        left_paddle_rect.h = left_paddle.h;

        SDL_FRect right_paddle_rect;
        right_paddle_rect.x = right_paddle.x;
        right_paddle_rect.y = right_paddle.y;
        right_paddle_rect.w = right_paddle.w;
        right_paddle_rect.h = right_paddle.h;

        SDL_FRect ball_rect;
        ball_rect.x = ball.x;
        ball_rect.y = ball.y;
        ball_rect.w = ball.w;
        ball_rect.h = ball.h;

        SDL_SetRenderDrawColor(renderer, 1, 1, 1, 255); // background
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 123, 123, 123, 255); // paddle color
        SDL_RenderFillRect(renderer, &left_paddle_rect);
        SDL_RenderFillRect(renderer, &right_paddle_rect);
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // ball color
        SDL_RenderFillRect(renderer, &ball_rect);

        // Render all text for scoring and win conditions
        const char* left_score_text = std::to_string(left_score).c_str();
        RenderText(renderer, font, left_score_text, 10, 10);
        const char* right_score_text = std::to_string(right_score).c_str();
        RenderText(renderer, font, right_score_text, window_width - 20, 10);
        if (left_score == 5){
            const char* left_win_text = "LEFT WINS!";
            RenderText(renderer, font, left_win_text, window_width / 2.0f, window_height / 2.0f);
        }
        if (right_score == 5){
            const char* right_win_text = "RIGHT WINS!";
            RenderText(renderer, font, right_win_text, window_width / 2.0f, window_height / 2.0f);
        }
        SDL_RenderPresent(renderer); // Send drawing to Screen
        
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
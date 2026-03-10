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


            if (ball.x <= left_paddle.w){
                if (ball.y >= left_paddle.y && ball.y <= left_paddle.h + left_paddle.y){
                    ball.x_velo *= -1;
                }
                if (ball.x <= 0){
                    right_score += 1;
                    score = true;
                }
            }

            if (ball.x >= window_width - right_paddle.w - ball.w){
                if (ball.y >= right_paddle.y && ball.y <= right_paddle.h + right_paddle.y){
                    ball.x_velo *= -1;
                }
                if (ball.x >= window_width){
                    left_score += 1;
                    score = true;
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

        // Handles drawing
        SDL_SetRenderDrawColor(renderer, 1, 1, 1, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 123, 123, 123, 255);
        SDL_RenderFillRect(renderer, &left_paddle_rect);

        SDL_SetRenderDrawColor(renderer, 123, 123, 123, 255);
        SDL_RenderFillRect(renderer, &right_paddle_rect);

        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderFillRect(renderer, &ball_rect);


        SDL_Color font_color = {255, 255, 255, 255};

        std::string left_score_str = std::to_string(left_score);
        const char* left_score_char = left_score_str.c_str();
        SDL_Surface* surface_left = TTF_RenderText_Blended(font, left_score_char, 0, font_color);
        SDL_Texture* texture_left = SDL_CreateTextureFromSurface(renderer, surface_left);
        SDL_DestroySurface(surface_left);
        SDL_FRect rect_text_left;
        rect_text_left.x = 10;
        rect_text_left.y = 10;
        rect_text_left.w = 0; // handled by get texture size
        rect_text_left.h = 0; // handled by get texture size
        SDL_GetTextureSize(texture_left, &rect_text_left.w, &rect_text_left.h);
        SDL_RenderTexture(renderer, texture_left, NULL, &rect_text_left);
        SDL_DestroyTexture(texture_left);

        std::string right_score_str = std::to_string(right_score);
        const char* right_score_char = right_score_str.c_str();
        SDL_Surface* surface_right = TTF_RenderText_Blended(font, right_score_char, 0, font_color);
        SDL_Texture* texture_right = SDL_CreateTextureFromSurface(renderer, surface_right);
        SDL_DestroySurface(surface_right);
        SDL_FRect rect_text_right;
        rect_text_right.x = window_width - 20;
        rect_text_right.y = 10;
        rect_text_right.w = 0; // handled by get texture size
        rect_text_right.h = 0; // handled by get texture size
        SDL_GetTextureSize(texture_right, &rect_text_right.w, &rect_text_right.h);
        SDL_RenderTexture(renderer, texture_right, NULL, &rect_text_right);
        SDL_DestroyTexture(texture_right);


        if (left_score == 5){
            const char* left_win_char = "LEFT WINS!";
            SDL_Surface* surface_left_win = TTF_RenderText_Blended(font, left_win_char, 0, font_color);
            SDL_Texture* texture_left_win = SDL_CreateTextureFromSurface(renderer, surface_left_win);
            SDL_DestroySurface(surface_left_win);
            SDL_FRect rect_text_left_win;
            rect_text_left_win.x = window_width / 2.0f;
            rect_text_left_win.y = window_height / 2.0f;
            rect_text_left_win.w = 0; // handled by get texture size
            rect_text_left_win.h = 0; // handled by get texture size
            SDL_GetTextureSize(texture_left_win, &rect_text_left_win.w, &rect_text_left_win.h);
            SDL_RenderTexture(renderer, texture_left_win, NULL, &rect_text_left_win);
            SDL_DestroyTexture(texture_left_win);

        }
        if (right_score == 5){
            const char* right_win_char = "RIGHT WINS!";
            SDL_Surface* surface_right_win = TTF_RenderText_Blended(font, right_win_char, 0, font_color);
            SDL_Texture* texture_right_win = SDL_CreateTextureFromSurface(renderer, surface_right_win);
            SDL_DestroySurface(surface_right_win);
            SDL_FRect rect_text_right_win;
            rect_text_right_win.x = window_width / 2.0f;
            rect_text_right_win.y = window_height / 2.0f;
            rect_text_right_win.w = 0; // handled by get texture size
            rect_text_right_win.h = 0; // handled by get texture size
            SDL_GetTextureSize(texture_right_win, &rect_text_right_win.w, &rect_text_right_win.h);
            SDL_RenderTexture(renderer, texture_right_win, NULL, &rect_text_right_win);
            SDL_DestroyTexture(texture_right_win);
        }

        SDL_RenderPresent(renderer);

        // SDL_Delay(32); // roughly 30fps
    }

    // Cleanup
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
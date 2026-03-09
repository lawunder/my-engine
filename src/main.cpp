#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>

int main(int argc, char* argv[])
{
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL failed to initialize: " << SDL_GetError() << "\n";
        return -1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("My Engine", 1280, 720, SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        std::cerr << "Failed to create window: " << SDL_GetError() << "\n";
        SDL_Quit();
        return -1;
    }

    std::cout << "Window created successfully, creating renderer\n";
    
    // Create Renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    
    if (!renderer)
    {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    std::cout << "Renderer created successfully, entering event loop\n";


    // Basic event loop - keeps window open
    bool running = true;
    SDL_Event event;

    float rectangle_x = 0;
    float rectangle_y = 0;
    float rectangle_width = 100;
    float rectangle_height = 200;
    float speed = 100.0f; // pixels per second
    float move;
    int current_time = SDL_GetTicks();
    int previous_time = SDL_GetTicks();

    while (running)
    {
        // Process events
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            if (event.type == SDL_EVENT_KEY_DOWN)
                if (event.key.key == SDLK_ESCAPE)
                    running = false;
            
            // std::cout << event.type << "\n";
        }

        current_time = SDL_GetTicks();
        float delta = current_time - previous_time;
        float delta_seconds = delta / 1000.0f;
        move = delta_seconds * speed;
        previous_time = current_time;

        const bool* keystate = SDL_GetKeyboardState(NULL);
        // Movement of rectangle
        if (keystate[SDL_SCANCODE_UP])
            rectangle_y -= move;
            // if (rectangle_y <= 0)
            //   rectangle_y = 0;
        if (keystate[SDL_SCANCODE_DOWN])
            rectangle_y += move;
        if (keystate[SDL_SCANCODE_LEFT])
            rectangle_x -= move;
        if (keystate[SDL_SCANCODE_RIGHT])
            rectangle_x += move;



        // Choose rectangle starting and shape
        SDL_FRect rect;
        rect.x = rectangle_x;
        rect.y = rectangle_y;
        rect.w = rectangle_width;
        rect.h = rectangle_height;

        // Handles drawing
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer);


        // SDL_Delay(32); // roughly 30fps
    }


    // Cleanup
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}
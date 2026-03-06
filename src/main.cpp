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
    SDL_Window* window = SDL_CreateWindow("My Engine", 1280, 720, 0);

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


    /*
    SDL_SCANCODE_RIGHT = 79,
    SDL_SCANCODE_LEFT = 80,
    SDL_SCANCODE_DOWN = 81,
    SDL_SCANCODE_UP = 82,
    */

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
        }


        const bool* keystate = SDL_GetKeyboardState(NULL);
        // Movement of rectangle
        if (keystate[SDL_SCANCODE_UP])
            rectangle_y -= 1;
        if (keystate[SDL_SCANCODE_DOWN])
            rectangle_y += 1;
        if (keystate[SDL_SCANCODE_LEFT])
            rectangle_x -= 1;
        if (keystate[SDL_SCANCODE_RIGHT])
            rectangle_x += 1;



        // Choose rectangle starting and shape
        SDL_FRect rect;
        rect.x = rectangle_x;
        rect.y = rectangle_y;
        rect.w = 100;
        rect.h = 200;

        // Handles drawing
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer);
    }


    // Cleanup
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}
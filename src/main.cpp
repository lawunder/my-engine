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
    SDL_Window* window = SDL_CreateWindow(
        "My Engine",    // title
        1280,720,       // width, height
        0               // flags
    );

    if (!window)
    {
        std::cerr << "Failed to create window: " << SDL_GetError() << "\n";
        SDL_Quit();
        return -1;
    }

    // Basic event loop - keeps window open
    bool running = true;
    SDL_Event event;


    std::cout << "Window created successfully, entering event loop\n";

    while (running)
    {
        // Process events
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            if (event.type == SDL_EVENT_KEY_DOWN)
                if (event.key.key = SDLK_ESCAPE)
                    running = false;
        }
    }



    // Cleanup
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
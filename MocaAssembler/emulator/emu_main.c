#include "emu_main.h"

void main()
{
    com_assert(SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) == 0, "Error Configuring.\n");

    /*
     *  Make sure TTF_Font is initialized to use.
     * */    
    TTF_Init();

    /*
     *  Initialize the device for BIOS.
     * */
    init_device("out.bin");

    window = SDL_CreateWindow("MocaEmulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, 
        SDL_WINDOW_SHOWN | SDL_WINDOW_MINIMIZED
    );
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    SDL_Surface *surface = SDL_GetWindowSurface(window);

    SDL_Event event;
    SDL_Rect rect = (SDL_Rect){
        .x = 50,
        .y = 50,
        .w = 50,
        .h = 50,
    };

    bool quit = false;
    bool init = false;

    while (!quit)
    {
        while(SDL_PollEvent(&event) == 1)
        {
            if(event.type == SDL_QUIT)
                quit = true;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        //SDL_RenderClear(renderer);

        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer);
        //SDL_RenderCopy(renderer, )
    }
}
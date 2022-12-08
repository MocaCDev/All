#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT (WINDOW_WIDTH)

/*
 * Information about the text
 *
 * */
typedef struct TxtInfo {
    SDL_Renderer *renderer;
    int x;
    int y;
    char *text;
    TTF_Font *font;
    SDL_Texture **texture;
    SDL_Rect *rect;
} _TxtInfo;

/*
- x, y: upper left corner.
- texture, rect: outputs.
*/
void get_text_and_rect(_TxtInfo text_info) {
    int text_width;
    int text_height;
    SDL_Surface *surface;
    SDL_Color textColor = {0, 0, 0, 0};

    surface = TTF_RenderText_Solid(text_info.font, text_info.text, textColor);
    *text_info.texture = SDL_CreateTextureFromSurface(text_info.renderer, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    text_info.rect->x = text_info.x;
    text_info.rect->y = text_info.y;
    text_info.rect->w = text_width;
    text_info.rect->h = text_height;
}

int main(int argc, char **argv) {
    SDL_Event event;
    SDL_Rect rect1, rect2;
    SDL_Renderer *renderer;
    SDL_Texture *texture1, *texture2;
    SDL_Window *window;
    char *font_path;
    int quit;

    if (argc == 1) {
        font_path = "font.ttf";
    } else if (argc == 2) {
        font_path = argv[1];
    } else {
        fprintf(stderr, "error: too many arguments\n");
        exit(EXIT_FAILURE);
    }

    /* Inint TTF. */
    SDL_Init(SDL_INIT_EVERYTHING);
    //SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
    window = SDL_CreateWindow(
        "Title",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    renderer = SDL_CreateRenderer(
        window, 
        -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    //SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window, &renderer);
    TTF_Init();
    TTF_Font *font = TTF_OpenFont(font_path, 24);
    if (font == NULL) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }
    get_text_and_rect((_TxtInfo){
        .renderer = renderer, 
        .x = 0, .y = 0, 
        .text = "HELLO!", 
        .font = font,
        .texture = &texture1, 
        .rect = &rect1
    });
    get_text_and_rect((_TxtInfo){
        renderer, 0, rect1.y + rect1.h, "WORLD", font, &texture2, &rect2
    });

    quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event) == 1) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        SDL_RenderClear(renderer);

        /* Use TTF textures. */
        SDL_RenderCopy(renderer, texture1, NULL, &rect1);
        SDL_RenderCopy(renderer, texture2, NULL, &rect2);

        SDL_RenderPresent(renderer);
    }

    /* Deinit TTF. */
    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
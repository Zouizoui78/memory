#include <time.h>

#include "Logger.hpp"
#include "Renderer.hpp"
#include "Memory.hpp"

int main(int argc, char*argv[])
{
    (void) argc;
    (void) argv;

    srand(time(NULL));
    SDL_Event event;

    logInit();

    Renderer r;
    if(!r.init())
        return -1;
    
    TTF_Font* font = r.loadFont("res/olivier.ttf", 40);
    if(font == nullptr)
        return -1;

    r.setDefaultFont(font);

    SDL_Texture* cardSpriteSheet = r.loadImage("res/cards.bmp");
    if(cardSpriteSheet == nullptr)
        return -1;

    SDL_Texture* background = r.loadImage("res/background.bmp");
    if(background == nullptr)
        return -1;

    Memory memory(&r, cardSpriteSheet, background);

    //Main loop.
    while(!memory.getQuit())
    {
        r.clear();

        //Event loop.
        while(SDL_PollEvent(&event) != 0)
            memory.eventHandler(event);

        memory.update();
        memory.render();
        r.refresh();
        SDL_Delay(10);
    }

    //Quit SDL.
    //Destroying textures
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(cardSpriteSheet);
    
    TTF_CloseFont(font);

    r.stop();
    return 0;
}
